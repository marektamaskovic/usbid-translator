#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <string>
#include <vector>

// WARNING
// uint16_t instead of uint8_t cause 4 numbers! Error in gist!

typedef struct {
	uint16_t id;
	std::string name;
} interface_t;

typedef struct {
	uint16_t id;
	std::string name;
	std::vector<interface_t> interfaces;
} device_t;

typedef struct {
	uint16_t id;
	std::string name;
	std::vector<device_t> devices;
} vendor_t;

typedef struct {
	uint16_t id;
	std::string name;
} hid_usage_t;

typedef struct {
	uint8_t id;
	std::string name;
	std::vector<hid_usage_t> usage;
} hid_usage_page_t;

typedef struct {
	std::vector<vendor_t> vendors;
	std::vector<hid_usage_page_t> hid_usage_pages;
} usb_ids_t;

#endif