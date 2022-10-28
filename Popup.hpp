#pragma once
#include <string>
#include <list>

class Popup
{
	static inline std::list<std::pair<int,std::u8string>> value;

public:
	static void push(int time, std::u8string text);
	static void update();
	static void draw();
};

