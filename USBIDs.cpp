#include "USBIDs.hpp"


template <typename T>
int insertInto(T &output, const std::string &line, const std::string &fmt_line, const unsigned prefix_len) {
	std::string name;
	uint32_t id;
	typename T::value_type item;

	std::sscanf(line.c_str(), fmt_line.c_str(), &id);
	name = line.substr(prefix_len);

	item.name = name;
	// converting from 32bit to 16bit integer;
	item.id = (id & 0x0000ffff); // TODO parametrize this boyo

	try{
		output.push_back(item);
	}
	catch(std::bad_alloc &e){
		std::cout << e.what() <<std::endl;
	}
	return 0;
}

template <typename T>
int insertInto(T &output, const std::string &num, const std::string &name){
	uint32_t id;
	typename T::value_type item;

	std::sscanf(num.c_str(), "%x", &id);
	item.name = name;
	item.id = id;
	try{
		output.emplace_back(item);
	}
	catch(std::bad_alloc &e){
		std::cout << e.what() <<std::endl;
	}
	return 0;
}


USBIDs::USBIDs(const std::string &filename){
	this->parseStream(filename);
}

std::string USBIDs::idToString(uint16_t vid, uint16_t pid){

	for(auto vendor_it = usb_info.vendors.begin();
		vendor_it != usb_info.vendors.end();
		++vendor_it)
	{
		if(vendor_it->id == vid && vendor_it->devices.size() != 0){
			for(auto device_it = vendor_it->devices.begin();
				device_it != vendor_it->devices.end();
				++device_it)
			{
				if(device_it->id == pid){
					return vendor_it->name + "::" + device_it->name;
				}
			}
		}
		else if(vendor_it->id == vid && vendor_it->devices.size() == 0){
			return vendor_it->name;
		}

	}

	return "Not found";
}


std::string USBIDs::interfaceToString(uint8_t c, uint8_t s, uint8_t p){

	for(auto name_it = this->usb_info.dev_class.begin();
		name_it != this->usb_info.dev_class.end();
		++name_it)
	{
		if(name_it->id == c && name_it->subclass.size() != 0){
			for(auto subname_it = name_it->subclass.begin();
				subname_it != name_it->subclass.end();
				++subname_it)
			{
				if(subname_it->id == s && subname_it->protocol.size() != 0){
					for(auto protocol_it = subname_it->protocol.begin();
						protocol_it != subname_it->protocol.end();
						++protocol_it)
					{
						if(protocol_it->id == p){
							return	name_it->name + "::"
									+ subname_it->name + "::"
									+ protocol_it->name;
						}
					}
				}
				else if(subname_it->id == s && subname_it->protocol.size() == 0){
					return name_it->name + "::" + subname_it->name;
				}
			}
		}
		else if(name_it->id == c && name_it->subclass.size() == 0){
			return name_it->name;
		}
	}

	return "Not found";
}

namespace usbid{

	using namespace pegtl;

	struct space2 : istring< ' ', ' ' > {};
	struct name : plus< not_one< '\n' > > {};
	struct number : if_must< xdigit > {};
	struct number2 : if_must< xdigit, xdigit > {};
	struct number3 : if_must< xdigit, xdigit, xdigit > {};
	struct number4 : if_must< xdigit, xdigit, xdigit, xdigit > {};

	struct comment : if_must< one< '#' >, until< eol > > {};
	struct empty_line : if_must<plus< sor< space, eol > >> {};
	struct nothing : sor< empty_line, comment > {};

	struct interface : if_must < two< '\t' >, number4, space2, name, eol > {};
	struct device : if_must< one<'\t'>, number4, space2, name, eol, star< interface > > {};
	struct vendor : if_must< number4, space2, name, eol > {};
	struct vdi : if_must< vendor, star< sor< device, comment > > > {};

	struct protocol : if_must< two<'\t'>, number2, space2, name, eol > {};
	struct subclass : if_must< one<'\t'>, number2, space2, name, eol > {};
	struct subcls : if_must< subclass, star< protocol > > {};
	struct cls : if_must< istring<'C',' '>, number2, space2, name, eol> {};
	struct csp : if_must< cls, star< subcls > > {};

	struct hut_page : if_must< istring<'H','U','T',' '>, number2, space2, name, eol > {};
	struct hut_usage : if_must< one<'\t'>, number3, space2, name, eol > {};
	struct hut: if_must< hut_usage, star< hut_page > > {};

	struct dialect: if_must< one<'\t'>, number2, space2, name, eol > {};
	struct lang: if_must< istring<'L', ' '>, number4, space2, name, eol > {};
	struct l : if_must< lang, star<dialect> > {};

	struct at : if_must< istring< 'A', 'T', ' '>, number4, space2, name, eol > {};
	struct hid : if_must< istring< 'H', 'I', 'D', ' '>, number2, space2, name, eol > {};
	struct r : if_must< istring< 'R', ' '>, number2, space2, name, eol > {};
	struct bias : if_must< istring< 'B', 'I', 'A', 'S', ' '>, number, space2, name, eol > {};
	struct phy : if_must< istring< 'P', 'H', 'Y', ' '>, number2, space2, name, eol > {};
	struct hcc : if_must< istring< 'H', 'C', 'C', ' '>, number2, space2, name, eol > {};
	struct vt : if_must< istring< 'A', 'T', ' '>, number4, space2, name, eol > {};

	struct something : sor< at, hid, r, bias, phy, hut, l, csp, vdi, hcc, vt, comment > {};

	struct anything: sor< something, nothing > {};
	struct grammar : until< eof, anything > {};




	template< typename Rule > struct action : pegtl::nothing< Rule > {};

	template<> struct action< empty_line > {
		static void apply(const pegtl::input & in, usb_ids_t & usb_ids) {
			// std::cout << "name: " << in.string() << std::endl;
			// std::cout << "empty_line:" << "->" << in.string() << "<-" << std::endl;
		}
	};

	template<> struct action< name > {
		static void apply(const pegtl::input & in, usb_ids_t & usb_ids) {
			// std::cout << "name: " << in.string() << std::endl;
			usb_ids.name_buf.push_back( in.string() );
		}
	};

	template<> struct action< number2 > {
		static void apply(const pegtl::input & in, usb_ids_t & usb_ids) {
			// std::cout << "num: " << in.string() << std::endl;
			usb_ids.num_buf.push_back( in.string() );
		}
	};

	template<> struct action< number3 > {
		static void apply(const pegtl::input & in, usb_ids_t & usb_ids) {
			// std::cout << "num: " << in.string() << std::endl;
			usb_ids.num_buf.push_back( in.string() );
		}
	};

	template<> struct action< number4 > {
		static void apply(const pegtl::input & in, usb_ids_t & usb_ids) {
			// std::cout << "num: " << in.string() << std::endl;
			usb_ids.num_buf.push_back( in.string() );
		}
	};

	template<> struct action< vendor > {
		static void apply(const pegtl::input & in, usb_ids_t & usb_ids) {
			// std::cout << "v:" << usb_ids.num_buf.back() << "  " << usb_ids.name_buf.back() << std::endl;
			insertInto(usb_ids.vendors, usb_ids.num_buf.back(), usb_ids.name_buf.back());
			usb_ids.buffer_pop();
		}
	};

	template<> struct action< device > {
		static void apply(const pegtl::input & in, usb_ids_t & usb_ids) {
			// std::cout << "d:\t" << usb_ids.num_buf.back() << "  " << usb_ids.name_buf.back() << std::endl;
			insertInto(usb_ids.vendors.back().devices, usb_ids.num_buf.back(), usb_ids.name_buf.back());
			usb_ids.buffer_pop();
		}
	};

	template<> struct action< cls > {
		static void apply(const pegtl::input & in, usb_ids_t & usb_ids) {
			// std::cout << "class:" << usb_ids.num_buf.back() << " " << usb_ids.name_buf.back() << std::endl;
			insertInto(
					usb_ids.dev_class,
					usb_ids.num_buf.back(),
					usb_ids.name_buf.back()
				);
			usb_ids.buffer_pop();
		}
	};

	template<> struct action< subclass > {
		static void apply(const pegtl::input & in, usb_ids_t & usb_ids) {
			// std::cout << "sclass:" << usb_ids.num_buf.back() << " " << usb_ids.name_buf.back() << std::endl;
			insertInto(
					usb_ids.dev_class.back().subclass,
					usb_ids.num_buf.back(),
					usb_ids.name_buf.back()
				);
			usb_ids.buffer_pop();
		}
	};

	template<> struct action< protocol > {
		static void apply(const pegtl::input & in, usb_ids_t & usb_ids) {
			// std::cout << "prot:" << usb_ids.num_buf.back() << " " << usb_ids.name_buf.back() << std::endl;
			insertInto(
					usb_ids.dev_class.back().subclass.back().protocol,
					usb_ids.num_buf.back(),
					usb_ids.name_buf.back()
				);
			usb_ids.buffer_pop();
		}
	};

	template<> struct action< comment > {
		static void apply(const pegtl::input & in, usb_ids_t & usb_ids) {
			// std::cout << "comment: '" << in.string() << "'" << std::endl;
		}
	};

	template< typename Rule >
		struct my_control: pegtl::normal< Rule > {
			static const std::string error_message;

			template< typename Input, typename ... States >
			static void raise( const Input & in, States && ... )
			{
			  throw pegtl::parse_error( error_message, in );
			}
		};

	template< typename T >
	const std::string my_control< T >::error_message =
	"parse error matching " + pegtl::internal::demangle< T >();
}

int USBIDs::parseStream(const std::string &filename){

	try {
		pegtl::file_parser( filename.c_str() ).parse<usbid::grammar, usbid::action, usbid::my_control>(usb_info);
	}
	catch(pegtl::parse_error &e){
			std::cerr << e.what() << std::endl;
	}

	return 0;
}


/* vim: set ts=4 sw=4 tw=0 noet : */