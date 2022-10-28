#include "Inventory.hpp"
#include "DataBase.hpp"
#include "Field.hpp"
#include "Input.hpp"
#include "Icon.hpp"
#include "convert_string.hpp"

Display Inventory::display({160,10}, {178,540}, 1);
bool Inventory::active = false;
int Inventory::column = 3;

short Inventory::qwerty[64] =
{
		18,	// 30	0
		0,	// 31	1
		2,	// 32	2
		4,	// 33	3
		6,	// 34	4
		8,	// 35	5
		10,	// 36	6
		12,	// 37	7
		14,	// 38	8
		16,	// 39	9
		-1,	// 3a	未定義
		-1,	// 3b	未定義
		-1,	// 3c	未定義
		-1,	// 3d	未定義
		-1,	// 3e	未定義
		-1,	// 3f	未定義
		-1,	// 40	未定義
		202,	// 41	A
		311,	// 42	B
		307,	// 43	C
		206,	// 44	D
		105,	// 45	E
		208,	// 46	F
		210,	// 47	G
		212,	// 48	H
		115,	// 49	I
		214,	// 4a	J
		216,	// 4b	K
		218,	// 4c	L
		315,	// 4d	M
		313,	// 4e	N
		117,	// 4f	O
		119,	// 50	P
		101,	// 51	Q
		107,	// 52	R
		204,	// 53	S
		109,	// 54	T
		113,	// 55	U
		309,	// 56	V
		103,	// 57	W
		305,	// 58	X
		111,	// 59	Y
		303,	// 5a	Z
		-1,	// 5b	左Windows
		-1,	// 5c	右Windows
		-1,	// 5d	アプリケーション
		-1,	// 5e	予約済
		-1,	// 5f	スリープ
		-1,	// 60	テンキー0
		-1,	// 61	テンキー1
		-1,	// 62	テンキー2
		-1,	// 63	テンキー3
		-1,	// 64	テンキー4
		-1,	// 65	テンキー5
		-1,	// 66	テンキー6
		-1,	// 67	テンキー7
		-1,	// 68	テンキー8
		-1,	// 69	テンキー9
		-1,	// 6a	乗算
		-1,	// 6b	キーの追加
		-1,	// 6c	区切り記号
		-1,	// 6d	減算
		-1,	// 6e	10進
		-1,	// 6f	キーの分割
};

void Inventory::controll(std::shared_ptr<Object>& player)
{
	if(Keyboard::push(VK_SHIFT))
	{
		Inventory::active = false;
		return;
	}
	if(player->status->second.item.empty())
		return;
	Point<int> p(select / column, select % column);
	--cool;
	if (Keyboard::push(VK_UP))
	{
		cool = 12;
		p.y -= 1;
		if (p.y < 0)
			p.y += (static_cast<int>(player->status->second.item.size()) - select % column - 1) / column + 1;
	}
	else if (Keyboard::press(VK_UP) && cool < 0)
	{
		cool = 4;
		p.y -= 1;
		if (p.y < 0)
			p.y += (static_cast<int>(player->status->second.item.size()) - select % column - 1) / column + 1;
	}
	else if (Keyboard::push(VK_DOWN))
	{
		cool = 12;
		p.y += 1;
		p.y %= (static_cast<int>(player->status->second.item.size()) - select % column - 1) / column + 1;
	}
	else if (Keyboard::press(VK_DOWN) && cool < 0)
	{
		cool = 4;
		p.y += 1;
		p.y %= (static_cast<int>(player->status->second.item.size()) - select % column - 1) / column + 1;
	}
	select = p.y * column;
	if (Keyboard::push(VK_LEFT))
	{
		cool = 12;
		p.x -= 1;
		if (p.x < 0)
		{
			p.x += column;
			if (select + p.x >= static_cast<int>(player->status->second.item.size()))
				p.y -= 1;
		}
	}
	else if (Keyboard::press(VK_LEFT) && cool < 0)
	{
		cool = 4;
		p.x -= 1;
		if (p.x < 0)
		{
			p.x += column;
			if (select + p.x >= static_cast<int>(player->status->second.item.size()))
				p.y -= 1;
		}
	}
	else if (Keyboard::push(VK_RIGHT))
	{
		cool = 12;
		p.x += 1;
		p.x %= column;
		if (select + p.x >= static_cast<int>(player->status->second.item.size()))
			p.y -= 1;
	}
	else if (Keyboard::press(VK_RIGHT) && cool < 0)
	{
		cool = 4;
		p.x += 1;
		p.x %= column;
		if (select + p.x >= static_cast<int>(player->status->second.item.size()))
			p.y -= 1;
	}
	select = p.y * column + p.x;
	
	if(Keyboard::press(VK_CONTROL))
	{
		for(int i = 0; i < 64; ++i)
		{
			if(Keyboard::push(i + 0x30))
			{
				auto it = player->status->second.item.begin();
				std::advance(it, select);
				static_cast<Player*>(player->status->first)->shortcut[i] = it->first;
				break;
			}
		}
	}
	else if(Keyboard::press(VK_SPACE) && player->status->second.cool <= 0)
	{
		auto it = player->status->second.item.begin();
		std::advance(it, select);
		auto item = DataBase::item.find(it->first);
		if(item != DataBase::item.end())
		{
			auto target = Field::get(player->pos, item->second.reach);
			if((target.expired() ? Action::execute(item->second.id, *player) : target.lock()->execute(item->second.id, *player)) && it->second != -1)
				if(--it->second == 0)
					player->status->second.item.erase(it);
			player->status->second.cool = 20;	// 仮
		}
	}
	select = __min(select, (int)player->status->second.item.size() - 1);
}

void Inventory::draw(const std::shared_ptr<Object>& player)
{
	if(active)
	{
		Point<int> boxSize(26, 180);

		if (player->status->second.item.empty())
		{
			display.DrawBox(0, 0, boxSize, 0xff888888, true);
			display.DrawRawString(3, 4, u8"アイテムを持っていない", 0xffffffff);
			display.DrawBox(0, 0, boxSize, 0xff6a6a6a, false);
		}
		else
		{
			int num = 0, start = __min(__max(select / column - 2, 0), (static_cast<int>(player->status->second.item.size()) - 1) / column - 4) * 3;
			const std::u8string* lore = nullptr;

			for (const auto& i : player->status->second.item)
			{
				auto item = DataBase::item.find(i.first);
				if (item != DataBase::item.end())
				{
					if (num >= start)
					{
						SetDrawBlendMode(DX_BLENDMODE_ALPHA, 196);
						display.DrawBox(boxSize.x * ((num-start) % column), boxSize.y * ((num - start) / column), boxSize, 0xff888888, true);
						SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
						display.DrawIcon(boxSize.x * ((num - start) % column) + 1, boxSize.y * ((num - start) / column) + 1, item->second.icon);
						display.DrawBox(boxSize.x * ((num - start) % column), boxSize.y * ((num - start) / column), boxSize, 0xff6a6a6a, false);
						display.DrawRawString(boxSize.x * ((num - start) % column) + boxSize.y + 3, boxSize.y * ((num - start) / column) + 4, item->second.name, 0xffffffff);
						if (i.second != -1)
							display.DrawRawString(boxSize.x * (((num - start) % column) + 1) - 5, boxSize.y * ((num - start) / column) + 4, ext::convert(std::to_string(i.second)), 0xffffffff, Ref::right);
						if (num == select)
						{
							SetDrawBlendMode(DX_BLENDMODE_ADD, 64);
							display.DrawBox(boxSize.x * ((num - start) % column), boxSize.y * ((num - start) / column), boxSize, 0xffffffff, true);
							lore = &item->second.lore;
						}
					}
					if (++num >= start + column * 5)
						break;
				}
			}
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 196);
			display.DrawBox(0, display.siz.y - 48, { 48,display.siz.x }, 0xff888888, true);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			if (lore != nullptr)
				display.DrawString(3, display.siz.y - 44, *lore, 0xffffffff);
			display.DrawBox(0, display.siz.y - 48, { 48,display.siz.x }, 0xff6a6a6a, false);
		}

		// ショートカット表示
		if(Keyboard::press(VK_CONTROL))
		{
			for(int i = 0; i < 64; ++i)
			{
				if(qwerty[i] != -1)
				{
					DrawBox(500 + (qwerty[i] % 100) * 13, 30 + (qwerty[i] / 100) * 26, 500 + (qwerty[i] % 100 + 2) * 13, 30 + (qwerty[i] / 100 + 1) * 26, 0xff888888, TRUE);
					DrawBox(500 + (qwerty[i] % 100) * 13, 30 + (qwerty[i] / 100) * 26, 500 + (qwerty[i] % 100 + 2) * 13, 30 + (qwerty[i] / 100 + 1) * 26, 0xff6a6a6a, FALSE);
					if (static_cast<Player*>(player->status->first)->shortcut[i] != -1)
					{
						Icon::draw(500 + (qwerty[i] % 100) * 13 + 1, 30 + (qwerty[i] / 100) * 26 + 1, DataBase::item.find(static_cast<Player*>(player->status->first)->shortcut[i])->second.icon);
					}
				}
			}
		}
	}
}

