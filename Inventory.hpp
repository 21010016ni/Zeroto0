#pragma once
#include <map>
#include "Display.hpp"
#include "Object.hpp"

class Inventory
{
	static Display display;

	static int column;

	static inline int cool = 0;

	static short qwerty[64];

public:
	static bool active;
	static inline int select = 0;

	static void controll(std::shared_ptr<Object>& player);
	static void draw(const std::shared_ptr<Object>& player);
};

