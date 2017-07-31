#include "USBIDs.hpp"

USBIDs::USBIDs(const std::string& filepath){

	std::fstream file {filepath};
	file.close();
	parseStream(filepath);
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
	std::regex vendor_line("^[a-f0-9]{4}\\s{2}\\w+",std::regex::ECMAScript | std::regex::icase);
	std::regex device_line("^\t[a-f0-9]{4}\\s{2}\\w+", std::regex::ECMAScript | std::regex::icase);
	std::regex interface_line("^\t\t[a-f0-9]{4}\\s{2}\\w+", std::regex::ECMAScript | std::regex::icase);

	// FIXME add switches for HID and other things.
	bool vendor_flag = false;
	bool hid_usage_flag = false;

	while(file.good()){
		std::getline(file, line);
		if(line[0] == '#') {
			std::cout << "comment: '" << line << "'" << std::endl;
			continue;
		}
		if (std::regex_search(line, vendor_line)) {
			// std::cout << "vendor: '" << line << "'" << std::endl;
			vendor_flag = true;
			parseVendor(this->vendor_list, line);
			std::cout << std::hex << (uint32_t)this->vendor_list.back().id
					  << "  " << this->vendor_list.back().name << std::endl;
		}
		else if(std::regex_search(line, device_line)){
			// std::cout << "device: '" << line << "'" << std::endl;
			parseDevice(this->vendor_list, line);
			std::cout << std::hex
					  << "\t"
					  << (uint32_t)this->vendor_list.back().devices.back().id
					  << "  "
					  << this->vendor_list.back().devices.back().name
					  << std::endl;
		}
		else if(std::regex_search(line, interface_line)){
			std::cout << "interface: '" << line << "'" << std::endl;
			parseInterface(this->vendor_list, line);
			std::cout << std::hex
					  << "\t\t"
					  << (uint32_t)this->vendor_list.back().devices.back().interfaces.back().id
					  << "  "
					  << this->vendor_list.back().devices.back().interfaces.back().name
					  << std::endl;
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

	std::sscanf(line.c_str(), "%x  %*s\n", &id);
	name = line.substr(5);

	vendor->name = name;
	// converting from 32bit to 16bit integer;
	vendor->id = (id & 0x0000ffff);

	vendor_list.push_back(*vendor);
	delete vendor;
	return 0;
}

int USBIDs::parseDevice(std::vector<vendor_t> &vendor_list, const std::string &line){
	// char name[120];
	std::string name;
	uint32_t id;
	device_t *device = new device_t;

	std::sscanf(line.c_str(), "\t%x  %*s\n", &id);
	name = line.substr(6);

	device->name = name;
	// converting from 32bit to 16bit integer;
	device->id = (id & 0x0000ffff);

	vendor_list.back().devices.push_back(*device);
	delete device;
	return 0;
}

int USBIDs::parseInterface(std::vector<vendor_t> &vendor_list, const std::string &line){
	// char name[120];
	std::string name;
	uint32_t id;
	interface_t *interface = new interface_t;

	std::sscanf(line.c_str(), "\t%x  %*s\n", &id);
	name = line.substr(6);

	interface->name = name;
	// converting from 32bit to 16bit integer;
	interface->id = (id & 0x0000ffff);

	vendor_list.back().devices.back().interfaces.push_back(*interface);
	delete interface;
	return 0;
}