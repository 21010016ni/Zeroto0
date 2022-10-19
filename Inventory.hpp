#pragma once
#include <map>
#include "Display.hpp"
#include "Object.hpp"

class Inventory
{
	static Display display;

	static int column;

	static inline int cool = 0;

public:
	static bool active;
	static inline int select = 0;

	static void controll(Player* target);
	static void draw(const Player* target);
};

