#pragma once
#include <string>
#include <vector>
#include "Display.hpp"

class Text
{
	Display display;

	std::u8string text;
	std::u8string drawText;
	float speed;
	float count;
	bool quick;
	bool active;
	size_t textPos;
	std::vector<std::u8string> elem;

	void getElem(std::vector<std::u8string>& result, const std::u8string& input, size_t& off)const;

public:
	Text(int x, int y, int w, int h, float speed) :display({ y,x }, { h,w }), speed(speed), count(0.0f), quick(false), active(false), textPos(INT64_MAX) {}

	void set(const char8_t* v);
	void update();
	void draw()const;
};

class TextManager
{
public:
	static Text player;
	static Text partner;

	static void update()
	{
		player.update();
		partner.update();
	}
	static void draw()
	{
		player.draw();
		partner.draw();
	}
};