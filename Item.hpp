#pragma once
#include <string>

class Item
{
public:
	std::u8string name;
	int id;
	int icon;
	int reach;

	Item(const char8_t* name, int id, int icon, int reach) :name(name), id(id), icon(icon), reach(reach) {};
};

