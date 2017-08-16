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


USBIDs::USBIDs(std::istream *input){
	this->parseStream(input);
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
	struct space2 : pegtl::two<' '> {};
	struct name : pegtl::plus<pegtl::print> {};
	struct number : pegtl::plus<pegtl::xdigit> {};
	struct device : pegtl::must<pegtl::one<'\t'>, number, space2, name, pegtl::eol> {};
	struct vendor : pegtl::must<number, space2, name, pegtl::eol> {};
	struct vdi : pegtl::plus<pegtl::plus<vendor>, pegtl::plus<device>> {};
	struct grammar : pegtl::must < vdi, pegtl::eof > {};

	template< typename Rule > struct action : pegtl::nothing< Rule > {};

}

int USBIDs::parseStream(std::istream *input){

	int line_num {0};
	std::string line;

	while(input->good()){
		line_num++;
		std::getline(*input, line);
		if(line[0] == '#') {
			// std::cout << "comment: '" << line << "'" << std::endl;
			continue;
		}
		else{
			try{
				pegtl::parse<usbid::grammar, usbid::action>(line, "line:" + std::to_string(line_num), this->usb_info);
			}
			catch(pegtl::parse_error &e){
				std::cerr << e.what() << std::endl;
				continue;
			}
		}
	}

	return 0;
}


/* vim: set ts=4 sw=4 tw=0 noet : */