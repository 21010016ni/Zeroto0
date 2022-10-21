#pragma once
#include <string>

class Item
{
public:
	std::u8string name;
	int icon;
	int id;
	int reach;

	Item(const char8_t* name, int icon, int id, int reach) :name(name), icon(icon), id(id), reach(reach) {};
};

