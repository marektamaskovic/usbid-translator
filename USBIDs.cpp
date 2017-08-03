#include "USBIDs.hpp"

USBIDs::USBIDs(const std::string& filepath){

	std::fstream file {filepath};
	this->parseStream(filepath);
	file.close();
}

std::string USBIDs::idToString(uint16_t vid, uint16_t pid){

	for(auto page_iter = usb_info.hid_usage_pages.begin();
		page_iter != usb_info.hid_usage_pages.end();
		++page_iter)
	{
		if(page_iter->id == vid && page_iter->usage.size() != 0){
			for(auto usage_iter = page_iter->usage.begin();
				usage_iter != page_iter->usage.end();
				++usage_iter)
			{
				if(usage_iter->id == pid){
					return page_iter->name + "::" + usage_iter->name;
				}
			}
		}
		else if(page_iter->id == vid && page_iter->usage.size() == 0){
			return page_iter->name;
		}
	}

	return "Not found!";
}

std::string USBIDs::interfaceToString(uint8_t c, uint8_t s, uint8_t p){

	for(auto vendor_it = usb_info.vendors.begin();
		vendor_it != usb_info.vendors.end();
		++vendor_it)
	{
		if(vendor_it->id == c && vendor_it->devices.size() != 0){
			for(auto device_it = vendor_it->devices.begin();
				device_it != vendor_it->devices.end();
				++device_it)
			{
				if(device_it->id == s && device_it->interfaces.size() != 0){
					for(auto interface_it = device_it->interfaces.begin();
						interface_it != device_it->interfaces.end();
						++interface_it)
					{
						if(interface_it->id == p){
							return vendor_it->name
								   + "::" + device_it->name
								   + "::" + interface_it->name;
						}
					}
				}
				else if(device_it->id == s && device_it->interfaces.size() == 0){
					return vendor_it->name + "::" + device_it->name;
				}
			}
		}
		else if(vendor_it->id == c && vendor_it->devices.size() == 0){
			return vendor_it->name;
		}

	}

	return "Not found";
}

#if 1
int USBIDs::parseStream(const std::string& filepath){
	std::fstream file {filepath};

	std::string line;

	std::regex vendor_line		("^[a-f0-9]{4}\\s{2}[\\W\\w]+", std::regex::ECMAScript | std::regex::icase | std::regex::optimize);
	std::regex device_line		("^\t[a-f0-9]{4}\\s{2}[\\W\\w]+", std::regex::ECMAScript | std::regex::icase | std::regex::optimize);
	std::regex interface_line	("^\t\t[a-f0-9]{4}\\s{2}[\\W\\w]+", std::regex::ECMAScript | std::regex::icase | std::regex::optimize);
	std::regex c_name			("^C\\s{1}[a-f0-9]{2}\\s{2}[\\W\\w\\?\\(\\)]+", std::regex::ECMAScript | std::regex::icase | std::regex::optimize);
	std::regex c_subname		("^\t[a-f0-9]{2}\\s{2}[\\W\\w]+", std::regex::ECMAScript | std::regex::icase | std::regex::optimize);
	std::regex c_protocol		("^\t\t[a-f0-9]{2}\\s{2}[\\W\\w]+", std::regex::ECMAScript | std::regex::icase | std::regex::optimize);
	std::regex at				("^AT\\s{1}[a-f0-9]{4}\\s{2}[\\W\\w]+", std::regex::ECMAScript | std::regex::icase | std::regex::optimize);
	std::regex hid				("^HID\\s{1}[a-f0-9]{2}\\s{2}[\\W\\w]+", std::regex::ECMAScript | std::regex::icase | std::regex::optimize);
	std::regex r				("^R\\s{1}[a-f0-9]{2}\\s{2}[\\W\\w]+", std::regex::ECMAScript | std::regex::icase | std::regex::optimize);
	std::regex bias				("^BIAS\\s{1}[a-f0-9]{1}\\s{2}[\\W\\w]+", std::regex::ECMAScript | std::regex::icase | std::regex::optimize);
	std::regex phy				("^PHY\\s{1}[a-f0-9]{2}\\s{2}[\\W\\w]+", std::regex::ECMAScript | std::regex::icase | std::regex::optimize);
	std::regex hut_page			("^HUT\\s{1}[a-f0-9]{2}\\s{2}[\\W\\w]+", std::regex::ECMAScript | std::regex::icase | std::regex::optimize);
	std::regex hut_usage		("^\t[a-f0-9]{3}\\s{2}[\\w\\W]+", std::regex::ECMAScript | std::regex::icase | std::regex::optimize);
	std::regex l_language		("^L\\s{1}[a-f0-9]{4}\\s{2}[\\W\\w]+", std::regex::ECMAScript | std::regex::icase | std::regex::optimize);
	std::regex l_dialect		("^\t[a-f0-9]{2}\\s{2}[\\W\\w]+", std::regex::ECMAScript | std::regex::icase | std::regex::optimize);
	std::regex hcc				("^HCC\\s{1}[a-f0-9]{2}\\s{2}[\\W\\w]+", std::regex::ECMAScript | std::regex::icase | std::regex::optimize);
	std::regex vt				("^VT\\s{1}[a-f0-9]{4}\\s{2}[\\W\\w]+", std::regex::ECMAScript | std::regex::icase | std::regex::optimize);



	bool last_language = false;
	uint64_t line_num = 0; // underflow

	while(file.good()){
		line_num++;
		std::getline(file, line);
		if(line[0] == '#') {
			// std::cout << "comment: '" << line << "'" << std::endl;
			continue;
		}
		if (std::regex_search(line, vendor_line)) {
			// std::cout << "vendor: '" << line << "'" << std::endl;
			// parseVendor(this->vendor_list, line);
			insertInto( this->usb_info.vendors,
						line,
						"%x  %*s\n",
						5);
		}
		else if(std::regex_search(line, device_line)){
			// std::cout << "device: '" << line << "'" << std::endl;
			// parseDevice(this->vendor_list, line);
			insertInto( this->usb_info.vendors.back().devices,
						line,
						"\t%x  %*s\n",
						6);
		}
		else if(std::regex_search(line, interface_line)){
			// std::cout << "interface: '" << line << "'" << std::endl;
			// parseInterface(this->vendor_list, line);
			insertInto(	this->usb_info.vendors.back().devices.back().interfaces,
						line,
						"\t\t%x  %*s\n",
						6);
		}
		else if(std::regex_search(line, c_name)){
			last_language = false;
			// std::cout << "c_name: '" << line << "'" << std::endl;
			insertInto( this->usb_info.dev_class,
						line,
						"C %x  %*s\n",
						6);
		}
		else if(!last_language && std::regex_search(line, c_subname)){
			// std::cout << "c_subname: '" << line << "'" << std::endl;
			insertInto( this->usb_info.dev_class.back().subclass,
						line,
						"\t%x  %*s",
						5);
		}
		else if(std::regex_search(line, c_protocol)){
			// std::cout << "c_protocol: '" << line << "'" << std::endl;
			// FIXME check if back() is a good iterator something is not pushing to vector!
			insertInto( this->usb_info.dev_class.back().subclass.back().protocol,
						line,
						"\t\t%x  %*s",
						6);
		}
		else if(std::regex_search(line, hut_page)){
			// std::cout << "hut_page: '" << line << "'" << std::endl;
			// parseHutPage(line);
			insertInto( this->usb_info.hid_usage_pages,
						line,
						"HUT %x  %*s\n",
						8);
		}
		else if(std::regex_search(line, hut_usage)){
			// std::cout << "hut_usage: '" << line << "'" << std::endl;
			// parseHutUsage(line);
			insertInto( this->usb_info.hid_usage_pages.back().usage,
						line,
						"\t%x  %*s\n",
						6);
		}
		else if(std::regex_search(line, at)){
			// std::cout << "at: '" << line << "'" << std::endl;
			insertInto( this->usb_info.actt,
						line,
						"AT %x  %*s\n",
						9);
		}
		else if(std::regex_search(line, hid)){
			// std::cout << "hid: '" << line << "'" << std::endl;
			insertInto( this->usb_info.hid_desc,
						line,
						"HID %x  %*s\n",
						8);
		}
		else if(std::regex_search(line, r)){
			// std::cout << "r: '" << line << "'" << std::endl;
			insertInto( this->usb_info.hid_item,
						line,
						"R %x  %*s\n",
						6);
		}
		else if(std::regex_search(line, bias)){
			// std::cout << "bias: '" << line << "'" << std::endl;
			insertInto( this->usb_info.bias,
						line,
						"BIAS %x  %*s\n",
						8);
		}
		else if(std::regex_search(line, phy)){
			// std::cout << "phy: '" << line << "'" << std::endl;
			insertInto( this->usb_info.phy_item,
						line,
						"PHY %x  %*s\n",
						8);
		}
		else if(std::regex_search(line, l_language)){
			// std::cout << "l_language: '" << line << "'" << std::endl;
			insertInto( this->usb_info.lang,
						line,
						"L %x  %*s\n",
						8);
			last_language = true;
		}
		else if(std::regex_search(line, l_dialect)){
			// std::cout << "l_dialect: '" << line << "'" << std::endl;
			insertInto( this->usb_info.lang.back().dialects,
						line,
						"\t%x  %*s\n",
						5);
		}
		else if(std::regex_search(line, hcc)){
			// std::cout << "hcc: '" << line << "'" << std::endl;
			insertInto( this->usb_info.hid_country,
						line,
						"HCC %x  %*s\n",
						8);
		}
		else if(std::regex_search(line, vt)){
			// std::cout << "vt: '" << line << "'" << std::endl;
			insertInto( this->usb_info.vctt,
						line,
						"VT %x  %*s\n",
						9);
		}
		else{
			// std::cout <<"nope: '" << line << "'" <<std::endl;
			if(line.length() > 0){
				std::cerr << filepath << ":"
						  << line_num << ": "
						  << "Syntax error"
						  << std::endl;
				// TODO
				// FIXME nejaky goto;
			}
		}

	}
	file.close();
	return 0;
}

#else
int USBIDs::parseStream(const std::string& filepath){
	//use reimplement w/ PEGTL parser.
	return 0;
}
#endif

/* vim: set ts=4 sw=4 tw=0 noet : */