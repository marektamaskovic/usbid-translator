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
	uint16_t id;
	std::string name;
	std::vector<hid_usage_t> usage;
} hid_usage_page_t;

typedef struct {
	uint8_t id;
	std::string name;
} protocol_t;

typedef struct {
	uint8_t id;
	std::string name;
	std::vector<protocol_t> protocol;
} subclass_t;

typedef struct {
	uint8_t id;
	std::string name;
	std::vector<subclass_t> subclass;
} dev_class_t;

typedef struct {
	uint16_t id;
	std::string name;
} actt_t;

typedef struct {
	uint8_t id;
	std::string name;
} hid_desc_t;

typedef struct {
	uint8_t id;
	std::string name;
} hid_item_t;

typedef struct {
	uint8_t id;
	std::string name;
} bias_t;

typedef struct {
	uint8_t id;
	std::string name;
} phy_item_t;

typedef struct {
	uint8_t id;
	std::string name;
} dialect_t;

typedef struct {
	uint16_t id;
	std::string name;
	std::vector<dialect_t> dialects;
} lang_t;

typedef struct {
	uint8_t id;
	std::string name;
} hid_country_code_t;

typedef struct {
	uint16_t id;
	std::string name;
} vctt_t;

typedef struct {
	std::vector<vendor_t> vendors;
	std::vector<dev_class_t> dev_class;
	std::vector<actt_t> actt;
	std::vector<hid_desc_t> hid_desc;
	std::vector<hid_item_t> hid_item;
	std::vector<bias_t> bias;
	std::vector<phy_item_t> phy_item;
	std::vector<hid_usage_page_t> hid_usage_pages;
	std::vector<lang_t> lang;
	std::vector<hid_country_code_t> hid_country;
	std::vector<vctt_t> vctt;
} usb_ids_t;


#endif

/* vim: set ts=4 sw=4 tw=0 noet : */
