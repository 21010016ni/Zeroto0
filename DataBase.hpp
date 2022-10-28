#pragma once
#include <vector>
#include <map>
#include "Item.hpp"
#include "Status.hpp"

class DataBase
{
public:
	static inline std::map<int, Item> item;
	static std::vector<Item> enemyAction;
	static std::vector<Enemy> enemy;

	static void LoadItem(const char* FileName);
	static void preset();
};

