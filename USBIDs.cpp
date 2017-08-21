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
		output.push_back(item);
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
	struct name : plus< print > {};
	struct number : plus< xdigit > {};

	struct vendor : if_must< number, space2, name, eol > {};
	struct device : if_must< space, number, space2, name > {};
	struct vdi : must< vendor, plus< device > > {};

	struct something : sor< vdi > {};

	struct comment : if_must< one< '#' >, until< eolf > > {};
	struct space : plus< sor< space > > {};
	struct nothing : sor< space, comment > {};

	struct anything: sor< something, nothing > {};
	struct grammar : until< eof, anything > {};




	template< typename Rule > struct action : pegtl::nothing< Rule > {};

	template<> struct action< name > {
		static void apply(const pegtl::input & in, usb_ids_t & usb_ids) {
			// std::cout << "name: " << in.string() << std::endl;
			usb_ids.name_buf.push_back( in.string() );
		}
	};

	template<> struct action< number > {
		static void apply(const pegtl::input & in, usb_ids_t & usb_ids) {
			// std::cout << "num: " << in.string() << std::endl;
			usb_ids.num_buf.push_back( in.string() );
		}
	};

	template<> struct action< vendor > {
		static void apply(const pegtl::input & in, usb_ids_t & usb_ids) {
			//usb_ids.num_buf.push_back( in.string() );
			// std::cout << "v:" << usb_ids.num_buf.back() << " " << usb_ids.name_buf.back() << std::endl;
			// std::cout << "v:" <<std::endl;
			insertInto(usb_ids.vendors, usb_ids.num_buf.back(), usb_ids.name_buf.back());
			usb_ids.name_buf.pop_back(); usb_ids.num_buf.pop_back();
		}
	};

	template<> struct action< device > {
		static void apply(const pegtl::input & in, usb_ids_t & usb_ids) {
			//usb_ids.num_buf.push_back( in.string() );
			// std::cout << usb_ids.num_buf.back() << " " << usb_ids.name_buf.back() << std::endl;
			// std::cout << "d:" <<std::endl;
			insertInto(usb_ids.vendors.back().devices, usb_ids.num_buf.back(), usb_ids.name_buf.back());
			usb_ids.name_buf.pop_back(); usb_ids.num_buf.pop_back();
		}
	};

	// template<> struct action< pegtl::eol > {
	// 	static void apply(const pegtl::input & in, usb_ids_t & usb_ids) {
	// 		//usb_ids.num_buf.push_back( in.string() );
	// 		// std::cout << "new line parsed" << std::endl;
	// 	}
	// };

	template<> struct action< pegtl::any > {
		static void apply(const pegtl::input & in, usb_ids_t & usb_ids) {
			//usb_ids.num_buf.push_back( in.string() );
			std::cout << "parsed: '" << in.string() << "'" << std::endl;
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