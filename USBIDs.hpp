#ifndef USBIDS_HPP
#define USBIDS_HPP

#include <string>
#include <fstream>
#include <iostream>
#include <regex>
#include <vector>

#include "types.hpp"

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

	output.push_back(item);
	return 0;
}

class USBIDs
{
public:
	USBIDs(const std::string& filepath = "/usr/share/hwdata/usb.ids");

	std::string idToString(uint16_t vid, uint16_t pid);
	std::string interfaceToString(uint16_t c, uint16_t s, uint16_t p);

private:
	int parseStream(const std::string &);

	std::vector<vendor_t> vendor_list;
	usb_ids_t usb_info;
};

#endif