#include "Inventory.hpp"
#include "DataBase.hpp"
#include "Field.hpp"
#include "Input.hpp"
#include "convert_string.hpp"

Display Inventory::display({ 10,10 }, { 494,540 });
bool Inventory::active = false;
int Inventory::column = 3;

void Inventory::controll(Player* player)
{
	--cool;
	if (Keyboard::press(VK_SPACE) && player->status->second.cool <= 0)
	{
		auto it = player->status->second.item.begin();
		std::advance(it, select);
		if (it->first < DataBase::item.size())
		{
			auto target = Field::get(player->pos, DataBase::item[it->first].reach);
			if ((target.expired() ? Action::execute(it->first, *player) : target.lock()->execute(DataBase::item[it->first].id, *player)) && it->second != -1)
				if (--it->second == 0)
					player->status->second.item.erase(it);
			player->status->second.cool = 20;	// ‰¼
		}
	}
	if (Keyboard::push(VK_UP))
	{
		if (select - column >= 0)
			select -= column;
		cool = 12;
	}
	else if (Keyboard::press(VK_UP) && cool < 0)
	{
		if (select - column >= 0)
			select -= column;
		cool = 4;
	}
	else if (Keyboard::push(VK_DOWN))
	{
		if (select + column < (int)player->status->second.item.size())
			select += column;
		cool = 12;
	}
	else if (Keyboard::press(VK_DOWN) && cool < 0)
	{
		if (select + column < (int)player->status->second.item.size())
			select += column;
		cool = 4;
	}
	if (Keyboard::push(VK_LEFT))
	{
		if (select % column != 0)
			select -= 1;
		cool = 12;
	}
	else if (Keyboard::press(VK_LEFT) && cool < 0)
	{
		if (select % column != 0)
			select -= 1;
		cool = 4;
	}
	else if (Keyboard::push(VK_RIGHT))
	{
		if (select % column != column - 1)
			select += 1;
		cool = 12;
	}
	else if (Keyboard::press(VK_RIGHT) && cool < 0)
	{
		if (select % column != column - 1)
			select += 1;
		cool = 4;
	}
	select = __min(select, (int)player->status->second.item.size() - 1);
}

void Inventory::draw(const Player* target)
{
	if (active)
	{
		int num = 0;
		Point<int> boxSize(26, 180);
		for (const auto& i : target->status->second.item)
		{
			if (i.first < DataBase::item.size())
			{
				display.DrawBox(boxSize.x * (num % column), boxSize.y * (num / column), boxSize, 0xff888888, TRUE);
				display.DrawIcon(boxSize.x * (num % column) + 1, boxSize.y * (num / column) + 1, DataBase::item[i.first].icon);
				display.DrawRawString(boxSize.x * (num % column) + boxSize.y + 3, boxSize.y * (num / column) + 4, DataBase::item[i.first].name, 0xffffffff);
				if (i.second != -1)
					display.DrawRawString(boxSize.x * ((num % column) + 1) - 5, boxSize.y * (num / column) + 4, ext::convert(std::to_string(i.second)), 0xffffffff, Ref::right);
				display.DrawBox(boxSize.x * (num % column), boxSize.y * (num / column), boxSize, 0xff6a6a6a, FALSE);
				if (num == select)
				{
					SetDrawBlendMode(DX_BLENDMODE_ADD, 64);
					display.DrawBox(boxSize.x * (num % column), boxSize.y * (num / column), boxSize, 0xffffffff, TRUE);
					SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
				}
				++num;
			}
		}
	}
}

