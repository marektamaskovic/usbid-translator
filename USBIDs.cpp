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
	namespace grammar{
		struct VCHAR : pegtl::internal::range< pegtl::internal::result_on_found::SUCCESS, pegtl::internal::peek_char, char( 33 ), char( 126 ) > {};
		struct WSP : pegtl::internal::one< pegtl::internal::result_on_found::SUCCESS, pegtl::internal::peek_char, ' ', '\t' > {};
		struct LF : pegtl::internal::one< pegtl::internal::result_on_found::SUCCESS, pegtl::internal::peek_char, '\n' > {};
		struct comment_cont : pegtl::until< pegtl::eol, pegtl::sor< WSP, VCHAR > > {};
		struct comment : pegtl::if_must< pegtl::one< '#' >, comment_cont > {};
		struct c_nl : pegtl::sor< comment, LF > {};
		struct c_wsp : pegtl::sor< WSP, pegtl::seq< c_nl, WSP > > {};

		struct space2 : pegtl::istring<' ', ' '> {};
		struct name : pegtl::plus<pegtl::print> {};
		struct number : pegtl::plus<pegtl::xdigit> {};
		struct line_inc : pegtl::success {};
		struct device : pegtl::must<line_inc, pegtl::istring<'\t'>, number, space2, name, pegtl::eol> {};
		struct vendor : pegtl::must<line_inc, number, space2, name, pegtl::eol> {};
		struct vdi : pegtl::plus<pegtl::plus<vendor>, pegtl::plus<device>> {};
		struct grammar : pegtl::until< pegtl::eof, pegtl::sor< pegtl::seq< pegtl::star< c_wsp >, c_nl >, pegtl::must< vdi > > > {};

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

	template< typename Rule > struct action : pegtl::nothing< Rule > {};

	template<> struct action< grammar::device >
	{
		static void apply( const pegtl::input & in, usb_ids_t &name/*std::string & name*/ ) {
			std::cout << "device: " << in.string() << std::endl;
		}
	};

	template<> struct action< grammar::vendor >
	{
		static void apply( const pegtl::input & in, usb_ids_t &name/*std::string & name*/ ) {
			std::cout << "device: " << in.string() << std::endl;
		}
	};

}

int USBIDs::parseStream(const std::string &filename){

	try {
		pegtl::file_parser( filename.c_str() ).parse<usbid::grammar::grammar, usbid::action, usbid::grammar::my_control>(usb_info);
	}
	catch(pegtl::parse_error &e){
			std::cerr << e.what() << std::endl;
	}

	return 0;
}


/* vim: set ts=4 sw=4 tw=0 noet : */
