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

void Manager::preset()
{
	//font = LoadFontDataToHandle((const char*)u8"data/font/NotoSansJPLight.dft");
	Icon::load("data/picture/icon.png", 16);

	Effect::load(LoadGraph("data/effect/pipo-btleffect001.png"), 5, 1, LoadSoundMem((const char*)u8"data/se/刀剣・斬る01.mp3"));

	player.reset(new Object(0, Status(10, 10, 0)));

	Field::set(player);
	Field::set(new Object(5));

	volume.mute &= 0b11111110;
}

void Manager::update()
{
	if(--player->status->second.cool <= 0)
	{
		if(Keyboard::press(0xc8))
		{
			auto i = Field::getIterator(player->pos, 4);
			if(i == Field::cend())
			{
				player->pos += 4;
			}
			else
			{
				if((*i)->status->second.flag & 1)
				{
					player->pos += 4;
					auto j = Field::getIterator(player);
					std::iter_swap(i, j);
				}
				else
				{
					player->pos = (*i)->pos;
				}
				// ここに接触時イベント
			}
			player->status->second.cool = 20;
		}
	}

	// BGM更新処理
	BGM::update();
	// ハンドル管理更新
	HandleManager::update();
}

void Manager::draw()
{
	for(auto i = Field::begin(); i != Field::cend(); ++i)
	{
		DrawCircle(10 + (*i)->pos * 10, 10, 8, 0xffffffff);
	}

	Effect::play();
	ParticleSystem::draw();
}

