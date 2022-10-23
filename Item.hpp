#pragma once
#include <string>

class Item
{
public:
	std::u8string name;
	std::u8string lore;
	int icon;
	int id;
	int reach;

	Item(const char8_t* name, const char8_t* lore, int icon, int id, int reach) :name(name), lore(lore), icon(icon), id(id), reach(reach) {};
};

