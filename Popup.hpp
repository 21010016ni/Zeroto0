#pragma once
#include <string>
#include <list>
#include "Display.hpp"

class Popup
{
	struct Data
	{
		int time;
		std::u8string left;
		std::u8string right;
	};
	static inline std::list<Data> value;
	static Display display;

public:
	static void push(int time, std::u8string left, std::u8string right = u8"");
	static void update();
	static void draw();
};

