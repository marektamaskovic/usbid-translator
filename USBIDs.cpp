#include "USBIDs.hpp"

USBIDs::USBIDs(const std::string& filepath){

	std::fstream file {filepath};
	parseStream(filepath);
	file.close();
}

std::string USBIDs::idToString(uint16_t vid, uint16_t pid){
	(void) vid;
	(void) pid;
	return "id";
}

std::string USBIDs::interfaceToString(uint8_t c, uint8_t s, uint8_t p){
	(void)c;
	(void)s;
	(void)p;
	return "interface";
}

#if 1
int USBIDs::parseStream(const std::string& filepath){
	std::fstream file {filepath};

	std::string line;

	std::regex vendor_line		("^[a-f0-9]{4}\\s{2}[\\W\\w]+", std::regex::ECMAScript | std::regex::icase | std::regex::optimize);
	std::regex device_line		("^\t[a-f0-9]{4}\\s{2}[\\W\\w]+", std::regex::ECMAScript | std::regex::icase | std::regex::optimize);
	std::regex interface_line	("^\t\t[a-f0-9]{4}\\s{2}[\\W\\w]+", std::regex::ECMAScript | std::regex::icase | std::regex::optimize);
	std::regex c_name			("^C\\s{1}[a-f0-9]{2}\\s{2}[\\W\\w]+", std::regex::ECMAScript | std::regex::icase | std::regex::optimize);
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



	// FIXME add switches for HID and other things.
	// bool vendor_flag = false;
	// bool hid_usage_flag = false;
	bool last_language = false;

	while(file.good()){
		std::getline(file, line);
		if(line[0] == '#') {
			std::cout << "comment: '" << line << "'" << std::endl;
			continue;
		}
		if (std::regex_search(line, vendor_line)) {
			// std::cout << "vendor: '" << line << "'" << std::endl;
			// vendor_flag = true;
			parseVendor(this->vendor_list, line);
			// std::cout << std::hex << (uint32_t)this->vendor_list.back().id
			// 		  << "  " << this->vendor_list.back().name << std::endl;
		}
		else if(std::regex_search(line, device_line)){
			// std::cout << "device: '" << line << "'" << std::endl;
			parseDevice(this->vendor_list, line);
			// std::cout << std::hex
			// 		  << "\t"
			// 		  << (uint32_t)this->vendor_list.back().devices.back().id
			// 		  << "  "
			// 		  << this->vendor_list.back().devices.back().name
			// 		  << std::endl;
		}
		else if(std::regex_search(line, interface_line)){
			// std::cout << "interface: '" << line << "'" << std::endl;
			parseInterface(this->vendor_list, line);
			// std::cout << std::hex
			// 		  << "\t\t"
			// 		  << (uint32_t)this->vendor_list.back().devices.back().interfaces.back().id
			// 		  << "  "
			// 		  << this->vendor_list.back().devices.back().interfaces.back().name
			// 		  << std::endl;
		}
		else if(std::regex_search(line, c_name)){
			last_language = false;
			std::cout << "c_name: '" << line << "'" << std::endl;
		}
		else if(!last_language && std::regex_search(line, c_subname)){
			std::cout << "c_subname: '" << line << "'" << std::endl;
		}
		else if(std::regex_search(line, c_protocol)){
			std::cout << "c_protocol: '" << line << "'" << std::endl;
		}
		else if(std::regex_search(line, hut_page)){
			// std::cout << "hut_page: '" << line << "'" << std::endl;
			parseHutPage(line);
		}
		else if(std::regex_search(line, hut_usage)){
			// std::cout << "hut_usage: '" << line << "'" << std::endl;
			parseHutUsage(line);
		}
		else if(std::regex_search(line, at)){
			std::cout << "at: '" << line << "'" << std::endl;
		}
		else if(std::regex_search(line, hid)){
			std::cout << "hid: '" << line << "'" << std::endl;
		}
		else if(std::regex_search(line, r)){
			std::cout << "r: '" << line << "'" << std::endl;
		}
		else if(std::regex_search(line, phy)){
			std::cout << "phy: '" << line << "'" << std::endl;
		}
		else if(std::regex_search(line, bias)){
			std::cout << "bias: '" << line << "'" << std::endl;
		}
		else if(std::regex_search(line, l_language)){
			std::cout << "l_language: '" << line << "'" << std::endl;
			last_language = true;
		}
		else if(std::regex_search(line, l_dialect)){
			std::cout << "l_dialect: '" << line << "'" << std::endl;
		}
		else if(std::regex_search(line, hcc)){
			std::cout << "hcc: '" << line << "'" << std::endl;
		}
		else if(std::regex_search(line, vt)){
			std::cout << "vt: '" << line << "'" << std::endl;
		}
		else{
			std::cout <<"nope: '" << line << "'" <<std::endl;
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

int USBIDs::parseVendor(std::vector<vendor_t> &vendor_list, const std::string &line){
	std::string name;
	uint32_t id;
	vendor_t *vendor = new vendor_t;
	(void) vendor_list;

	std::sscanf(line.c_str(), "%x  %*s\n", &id);
	name = line.substr(5);

	vendor->name = name;
	// converting from 32bit to 16bit integer;
	vendor->id = (id & 0x0000ffff);

	this->usb_info.vendors.push_back(*vendor);
	delete vendor;
	return 0;
}

int USBIDs::parseDevice(std::vector<vendor_t> &vendor_list, const std::string &line){
	// char name[120];
	std::string name;
	uint32_t id;
	device_t *device = new device_t;
	(void) vendor_list;

	std::sscanf(line.c_str(), "\t%x  %*s\n", &id);
	name = line.substr(6);

	device->name = name;
	// converting from 32bit to 16bit integer;
	device->id = (id & 0x0000ffff);

	this->usb_info.vendors.back().devices.push_back(*device);
	delete device;
	return 0;
}

int USBIDs::parseInterface(std::vector<vendor_t> &vendor_list, const std::string &line){
	// char name[120];
	std::string name;
	uint32_t id;
	interface_t *interface = new interface_t;
	(void) vendor_list;

	std::sscanf(line.c_str(), "\t%x  %*s\n", &id);
	name = line.substr(6);

	interface->name = name;
	// converting from 32bit to 16bit integer;
	interface->id = (id & 0x0000ffff);

	this->usb_info.vendors.back().devices.back().interfaces.push_back(*interface);
	delete interface;
	return 0;
}

int USBIDs::parseHutPage(const std::string &line){
	std::string name;
	uint32_t id;
	hid_usage_page_t *h_page = new hid_usage_page_t;

	std::sscanf(line.c_str(), "HUT %x  %*s\n", &id);
	name = line.substr(8);

	h_page->name = name;
	// converting from 32bit to 8bit integer;
	h_page->id = (id & 0x000000ff);

	this->usb_info.hid_usage_pages.push_back(*h_page);
	// std::cout << "H_Page: "
	// 		  << std::hex
	// 		  << (uint32_t)h_page->id
	// 		  << "  "
	// 		  << h_page->name
	// 		  << std::endl;
	delete h_page;
	return 0;
}

int USBIDs::parseHutUsage(const std::string &line){
	std::string name;
	uint32_t id;
	hid_usage_t *h_usage = new hid_usage_t;

	std::sscanf(line.c_str(), "\t%x  %*s\n", &id);
	name = line.substr(6);

	h_usage->name = name;
	// converting from 32bit to 16bit integer;
	h_usage->id = (id & 0x0000ffff);

	this->usb_info.hid_usage_pages.back().usage.push_back(*h_usage);
	// std::cout << "H_Page: "
	// 		  << std::hex
	// 		  << (uint32_t)h_usage->id
	// 		  << "  "
	// 		  << h_usage->name
	// 		  << std::endl;
	delete h_usage;
	return 0;
}
