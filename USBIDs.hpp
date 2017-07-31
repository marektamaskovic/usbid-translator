#ifndef USBIDS_HPP
#define USBIDS_HPP

#include <string>
#include <fstream>
#include <iostream>
#include <regex>
#include <vector>

#include "parser.hpp"

class USBIDs
{
public:
	USBIDs(const std::string& filepath = "/usr/share/hwdata/usb.ids");

	std::string idToString(uint16_t vid, uint16_t pid);
	std::string interfaceToString(uint8_t c, uint8_t s, uint8_t p);

private:
	int parseStream(const std::string &);
	int parseVendor(std::vector<vendor_t> &, const std::string &);
	int parseDevice(std::vector<vendor_t> &, const std::string &);
	int parseInterface(std::vector<vendor_t> &, const std::string &);

	std::vector<vendor_t> vendor_list;
};

#endif