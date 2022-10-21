#pragma once
#include <vector>
#include "Item.hpp"

class DataBase
{
public:
	static inline std::vector<Item> item;

	static void LoadItem(const char* FileName);
};

