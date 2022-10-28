#include "Popup.hpp"
#include <DxLib.h>

void Popup::push(int time, std::u8string text)
{
	value.emplace_front(time, text);
}

void Popup::update()
{
	for (auto i = value.begin();i!=value.end();)
	{
		if (--i->first <= 0)
		{
			i = value.erase(i);
			continue;
		}
		++i;
	}
}

void Popup::draw()
{
	for (const auto& i : value)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, i.first * 2);
		DrawBox(0, 600, 120, 600 - 26, 0xff888888, TRUE);
	}
}

