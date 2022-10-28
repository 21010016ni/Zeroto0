#include "Popup.hpp"
#include <DxLib.h>

Display Popup::display({0,0}, {600,160}, 2);

void Popup::push(int time, std::u8string left,std::u8string right)
{
	value.emplace_front(time, left, right);
}

void Popup::update()
{
	for (auto i = value.begin();i!=value.end();)
	{
		if (--i->time <= 0)
		{
			i = value.erase(i);
			continue;
		}
		++i;
	}
}

void Popup::draw()
{
	int y = display.siz.y;
	for (const auto& i : value)
	{
		y -= 26;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, i.time * 3);
		display.DrawBox(0, y, { 26,display.siz.x }, 0xff888888, TRUE);
		if (!i.left.empty())
		display.DrawRawString(3, y + 4, i.left, 0xffffffff);
		if (!i.right.empty())
			display.DrawRawString(display.siz.x - 3, y + 4, i.right, 0xffffffff, Ref::right);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

