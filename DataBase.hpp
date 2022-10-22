#pragma once
#include <vector>
#include "Item.hpp"
#include "Status.hpp"

class DataBase
{
public:
	static inline std::vector<Item> item;
	static std::vector<Item> enemyAction;
	static std::vector<Status> status;

	static void LoadItem(const char* FileName);
};

