#include "Inventory.hpp"
#include "DataBase.hpp"
#include "Field.hpp"
#include "Input.hpp"
#include "Icon.hpp"
#include "convert_string.hpp"

Display Inventory::display({10,10}, {494,540});
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
		328,	// 60	テンキー0
		322,	// 61	テンキー1
		324,	// 62	テンキー2
		326,	// 63	テンキー3
		222,	// 64	テンキー4
		224,	// 65	テンキー5
		226,	// 66	テンキー6
		122,	// 67	テンキー7
		124,	// 68	テンキー8
		126,	// 69	テンキー9
		26,	// 6a	乗算
		128,	// 6b	キーの追加
		-1,	// 6c	区切り記号
		28,	// 6d	減算
		228,	// 6e	10進
		24,	// 6f	キーの分割
};

void Inventory::controll(Player* player)
{
	--cool;
	if(Keyboard::push(VK_UP))
	{
		if(select - column >= 0)
			select -= column;
		cool = 12;
	}
	else if(Keyboard::press(VK_UP) && cool < 0)
	{
		if(select - column >= 0)
			select -= column;
		cool = 4;
	}
	else if(Keyboard::push(VK_DOWN))
	{
		if(select + column < (int)player->status->second.item.size())
			select += column;
		cool = 12;
	}
	else if(Keyboard::press(VK_DOWN) && cool < 0)
	{
		if(select + column < (int)player->status->second.item.size())
			select += column;
		cool = 4;
	}
	if(Keyboard::push(VK_LEFT))
	{
		if(select % column != 0)
			select -= 1;
		cool = 12;
	}
	else if(Keyboard::press(VK_LEFT) && cool < 0)
	{
		if(select % column != 0)
			select -= 1;
		cool = 4;
	}
	else if(Keyboard::push(VK_RIGHT))
	{
		if(select % column != column - 1)
			select += 1;
		cool = 12;
	}
	else if(Keyboard::press(VK_RIGHT) && cool < 0)
	{
		if(select % column != column - 1)
			select += 1;
		cool = 4;
	}
	if(Keyboard::press(VK_CONTROL))
	{
		for(int i = 0; i < 64; ++i)
		{
			if(Keyboard::push(i + 0x30))
			{
				auto it = player->status->second.item.begin();
				std::advance(it, select);
				player->shortcut[i] = it->first;
				break;
			}
		}
	}
	else if(Keyboard::press(VK_SPACE) && player->status->second.cool <= 0)
	{
		auto it = player->status->second.item.begin();
		std::advance(it, select);
		if(it->first < DataBase::item.size())
		{
			auto target = Field::get(player->pos, DataBase::item[it->first].reach);
			if((target.expired() ? Action::execute(DataBase::item[it->first].id, *player) : target.lock()->execute(DataBase::item[it->first].id, *player)) && it->second != -1)
				if(--it->second == 0)
					player->status->second.item.erase(it);
			player->status->second.cool = 20;	// 仮
		}
	}
	select = __min(select, (int)player->status->second.item.size() - 1);
}

void Inventory::draw(const Player* player)
{
	if(active)
	{
		int num = 0;
		Point<int> boxSize(26, 180);
		for(const auto& i : player->status->second.item)
		{
			if(i.first < DataBase::item.size())
			{
				display.DrawBox(boxSize.x * (num % column), boxSize.y * (num / column), boxSize, 0xff888888, TRUE);
				display.DrawIcon(boxSize.x * (num % column) + 1, boxSize.y * (num / column) + 1, DataBase::item[i.first].icon);
				display.DrawRawString(boxSize.x * (num % column) + boxSize.y + 3, boxSize.y * (num / column) + 4, DataBase::item[i.first].name, 0xffffffff);
				if(i.second != -1)
					display.DrawRawString(boxSize.x * ((num % column) + 1) - 5, boxSize.y * (num / column) + 4, ext::convert(std::to_string(i.second)), 0xffffffff, Ref::right);
				display.DrawBox(boxSize.x * (num % column), boxSize.y * (num / column), boxSize, 0xff6a6a6a, FALSE);
				if(num == select)
				{
					SetDrawBlendMode(DX_BLENDMODE_ADD, 64);
					display.DrawBox(boxSize.x * (num % column), boxSize.y * (num / column), boxSize, 0xffffffff, TRUE);
					SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
				}
				++num;
			}
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
					if(player->shortcut[i] != -1)
					{
						Icon::draw(500 + (qwerty[i] % 100) * 13 + 1, 30 + (qwerty[i] / 100) * 26 + 1, DataBase::item[player->shortcut[i]].icon);
					}
				}
			}
		}
	}
}

