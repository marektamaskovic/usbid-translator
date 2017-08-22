#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <string>
#include <vector>

struct interface_t{
	uint16_t id;
	std::string name;
};

struct device_t{
	uint16_t id;
	std::string name;
	std::vector<interface_t> interfaces;
};

struct vendor_t{
	uint16_t id;
	std::string name;
	std::vector<device_t> devices;
};

struct hid_usage_t{
	uint16_t id;
	std::string name;
};

struct hid_usage_page_t{
	uint8_t id;
	std::string name;
	std::vector<hid_usage_t> usage;
};

struct protocol_t{
	uint8_t id;
	std::string name;
};

struct subclass_t{
	uint8_t id;
	std::string name;
	std::vector<protocol_t> protocol;
};

struct dev_class_t{
	uint8_t id;
	std::string name;
	std::vector<subclass_t> subclass;
};

struct actt_t{
	uint16_t id;
	std::string name;
};

struct hid_desc_t{
	uint8_t id;
	std::string name;
};

struct hid_item_t{
	uint8_t id;
	std::string name;
};

struct bias_t{
	uint8_t id;
	std::string name;
};

struct phy_item_t{
	uint8_t id;
	std::string name;
};

struct dialect_t{
	uint8_t id;
	std::string name;
};

struct lang_t{
	uint16_t id;
	std::string name;
	std::vector<dialect_t> dialects;
};

struct hid_country_code_t{
	uint8_t id;
	std::string name;
};

struct vctt_t{
	uint16_t id;
	std::string name;
};

struct usb_ids_t{
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

	std::vector<std::string> num_buf;
	std::vector<std::string> name_buf;

	void buffer_pop(void){
	this->name_buf.pop_back();
	this->num_buf.pop_back();
}
};



struct params_t{
	bool id_f {false};
	bool interface_f {false};
	bool help_f {false};
	uint16_t id[2] {0,};
	uint8_t interface[3] {0,};
	std::string filepath {""};
};



#endif

/* vim: set ts=4 sw=4 tw=0 noet : */
