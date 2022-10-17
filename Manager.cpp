#include "Manager.hpp"
#include <DxLib.h>
#include "Particle.hpp"
#include "Icon.hpp"
#include "BGM.hpp"
#include "Input.hpp"
#include "Effect.hpp"
#include "HandleManager.hpp"
#include "convert_string.hpp"

void Manager::preset()
{
	//font = LoadFontDataToHandle((const char*)u8"data/font/NotoSansJPLight.dft");
	Icon::load("data/picture/icon.png", 16);

	Effect::load(LoadGraph("data/effect/pipo-btleffect001.png"), 5, 1, LoadSoundMem((const char*)u8"data/se/刀剣・斬る01.mp3"));

	volume.mute &= 0b11111110;
}

void Manager::update()
{
	// BGM更新処理
	BGM::update();
	// ハンドル管理更新
	HandleManager::update();
}

void Manager::draw()
{
	Effect::play();
	ParticleSystem::draw();
}

