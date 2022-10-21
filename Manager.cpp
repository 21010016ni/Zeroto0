#include "Manager.hpp"
#include <DxLib.h>
#include "Particle.hpp"
#include "Icon.hpp"
#include "BGM.hpp"
#include "Input.hpp"
#include "Effect.hpp"
#include "HandleManager.hpp"
#include "convert_string.hpp"
#include "Field.hpp"
#include "DataBase.hpp"
#include "Display.hpp"
#include "Inventory.hpp"
#include "Text.hpp"

void Manager::preset()
{
	font = LoadFontDataToHandle((const char*)u8"data/font/刻明朝18.dft");
	Display::SetFont(font);
	Icon::load("data/picture/icon.png", 16);
	DataBase::LoadItem("data/item.dat");

	Effect::load(LoadGraph("data/effect/pipo-btleffect001.png"), 5, 1, LoadSoundMem((const char*)u8"data/se/刀剣・斬る01.mp3"));

	static Status player_status(10, 10, u8"data/picture/flower0497.png", 0);
	//player.reset(new Player(0, player_status,10));
	//static_cast<Player*>(player.get())->shortcut[0x2a] = 0;
	//player->status->second.item.emplace(0, 10);
	//player->status->second.item.emplace(1, 10);
	//player->status->second.item.emplace(2, 10);
	//player->status->second.item.emplace(3, 10);
	//player->status->second.item.emplace(4, 10);
	//player->status->second.item.emplace(5, 10);
	//player->status->second.item.emplace(6, 10);
	//player->status->second.item.emplace(7, 10);
	//player->status->second.item.emplace(8, 10);
	//player->status->second.item.emplace(9, 10);
	//player->status->second.item.emplace(10, 10);
	//player->status->second.item.emplace(11, 10);
	//player->status->second.item.emplace(12, 10);

	static Status enemy_status(10, 10, u8"data/picture/flower3868.png", 1);

	Field::set(new Player(0, &player_status, 10));
	player = *Field::begin();
	Field::set(new Object(15, &enemy_status));

	volume.mute &= 0b11111110;
}

void Manager::update()
{
	--player->status->second.cool;
	if (Inventory::active)
	{
		if (Keyboard::push(VK_SHIFT))
		{
			Inventory::active = false;
		}
		Inventory::controll(static_cast<Player*>(player.get()));
	}
	else if (player->status->second.cool <= 0)
	{
		if (Keyboard::push(VK_SHIFT))
		{
			Inventory::active = true;
		}
		else if (Keyboard::press(VK_UP))
		{
			// 前進
			auto i = Field::getIterator(player->pos, 4, true);
			if (i == Field::cend())
			{
				player->pos += 4;
			}
			else
			{
				if (!(**i) || (*i)->status->second.flag & 1)
				{
					player->pos += 4;
					auto j = Field::getIterator(player);
					std::iter_swap(i, j);
				}
				else
				{
					player->pos = (*i)->pos - 1;
				}
				// ここに接触時イベント
			}
			player->status->second.cool = 20;	// 仮
		}
		else if (Keyboard::press(VK_DOWN))
		{
			// 後退
			auto i = Field::getIterator(player->pos, -4, true);
			player->pos -= 4;
			if (i != Field::cend())
			{
				auto j = Field::getIterator(player);
				std::iter_swap(i, j);
			}
			player->status->second.cool = 20;	// 仮
		}
		else
		{
			// アイテムショートカットの実行
			for (BYTE i = 0; i < 64; ++i)
				if (Keyboard::press(0x30 + i))
				{
					int id = static_cast<Player*>(player.get())->shortcut[i];
					if (id != -1 && id < DataBase::item.size())
					{
						auto item = player->status->second.item.find(id);
						if (item != player->status->second.item.cend())
						{
							auto target = Field::get(player->pos, DataBase::item[id].reach);
							if ((target.expired() ? Action::execute(DataBase::item[id].id, *player) : target.lock()->execute(DataBase::item[id].id, *player)) && item->second != -1)
								if (--item->second == 0)
									player->status->second.item.erase(item);
							player->status->second.cool = 20;	// 仮
						}
					}
				}
		}
	}

	TextManager::update();

	// BGM更新処理
	BGM::update();
	// ハンドル管理更新
	HandleManager::update();
}

void Manager::draw()
{
	//auto it = Field::getIterator(player);
	//++it;
	auto it = Field::cend();
	while (it != Field::begin() && (*--it)->pos > player->pos + static_cast<Player*>(player.get())->searchRange);
	while ((*it)->pos > player->pos && (*it)->pos <= player->pos + static_cast<Player*>(player.get())->searchRange)
	{
		DrawGraph(0, 0, HandleManager::get((*it)->status->first->graph, HandleManager::Type::graph), TRUE);
		if (it == Field::begin())
			break;
		--it;
	}

	for (auto i = Field::begin(); i != Field::cend(); ++i)
	{
		DrawCircle(10 + (*i)->pos * 10, 10, 8, 0xffffffff);
	}

	Inventory::draw(static_cast<Player*>(player.get()));

	TextManager::draw();

	Effect::play();
	ParticleSystem::draw();

	//SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	//DrawBox(0, 0, 1024, 600, 0xff000000, TRUE);
	//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//for(int i = 0; i < 256; ++i)
	//{
	//	DrawFormatString(100 * (i / 32), 18 * (i % 32), 0xffffffff, "%02x : %02x", i, Keyboard::get(i));
	//}
}

