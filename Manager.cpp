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

void Manager::preset()
{
	font = LoadFontDataToHandle((const char*)u8"data/font/刻明朝18.dft");
	Display::SetFont(font);
	Icon::load("data/picture/icon.png", 16);

	Effect::load(LoadGraph("data/effect/pipo-btleffect001.png"), 5, 1, LoadSoundMem((const char*)u8"data/se/刀剣・斬る01.mp3"));

	player.reset(new Player(0, Status(10, 10, 0)));
	static_cast<Player*>(player.get())->shortcut[0x2a] = 0;
	player->status->second.item.emplace(0, 10);
	player->status->second.item.emplace(1, 10);
	player->status->second.item.emplace(2, 10);
	player->status->second.item.emplace(3, 10);
	player->status->second.item.emplace(4, 10);
	player->status->second.item.emplace(5, 10);
	player->status->second.item.emplace(6, 10);
	player->status->second.item.emplace(7, 10);
	player->status->second.item.emplace(8, 10);
	player->status->second.item.emplace(9, 10);
	player->status->second.item.emplace(10, 10);
	player->status->second.item.emplace(11, 10);
	player->status->second.item.emplace(12, 10);

	Field::set(player);
	Field::set(new Object(15, Status(10, 10, 0)));

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
							if ((target.expired() ? Action::execute(id, *player) : target.lock()->execute(DataBase::item[id].id, *player)) && item->second != -1)
								if (--item->second == 0)
									player->status->second.item.erase(item);
							player->status->second.cool = 20;	// 仮
						}
					}
				}
		}
	}

	// BGM更新処理
	BGM::update();
	// ハンドル管理更新
	HandleManager::update();
}

void Manager::draw()
{
	for (auto i = Field::begin(); i != Field::cend(); ++i)
	{
		DrawCircle(10 + (*i)->pos * 10, 10, 8, 0xffffffff);
	}

	Inventory::draw(static_cast<Player*>(player.get()));

	//for(int i = 0; i < 256; ++i)
	//{
	//	DrawFormatString(20 * (i / 32), 10 * (i % 32), 0xffffffff, "%d", Keyboard::get(i));
	//}

	Effect::play();
	ParticleSystem::draw();
}

