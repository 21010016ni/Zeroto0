#pragma once
#include <vector>
#include <map>
#include "Action.hpp"
#include "Item.hpp"
#include "Status.hpp"

class DataBase
{
public:
	static std::vector<Action> action;
	static inline std::map<int, Item> item;
	static std::map<int, Enemy> enemy;
	static Player player;

	static inline int gameFlag = 0;

	static void LoadItem(const char* FileName);
};

