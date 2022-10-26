#include "Manager.hpp"
#include <DxLib.h>
#include "Icon.hpp"
#include "BGM.hpp"
#include "Input.hpp"
#include "Effect.hpp"
#include "HandleManager.hpp"
#include "convert_string.hpp"
#include "Field.hpp"
#include "DataBase.hpp"
#include "Inventory.hpp"
#include "Text.hpp"
#include "Config.hpp"
#include "ParticleDust.hpp"

#include <iostream>

Display Manager::display({0,0}, Application::WindowSize, 0);
Display Manager::ui({0,0}, Application::WindowSize, 2);

void Manager::preset()
{
	font = LoadFontDataToHandle((const char*)u8"data/font/刻明朝18.dft");
	back = LoadGraph((const char*)u8"data/picture/back.jpg");
	Display::SetFont(font);
	Icon::load("data/picture/icon.png", 16);
	DataBase::LoadItem("data/item.dat");
	BGM::set(u8"data/bgm/夢.mp3");
	BGM::set(u8"data/bgm/水に沈むピアノ.mp3");
	BGM::set(u8"data/bgm/レイト・ナイト・スノウ.mp3");
	BGM::set(u8"data/bgm/深海魚の遊泳.mp3");

	Effect::load(LoadGraph("data/effect/pipo-btleffect001.png"), 5, 1, LoadSoundMem((const char*)u8"data/se/刀剣・斬る01.mp3"));

	player = Field::set(new Player(0, &DataBase::status[0], 0, 10));
	Field::set(new Object(15, &DataBase::status[1]))->status->second.item.emplace(0, -1);
	static_cast<Player*>(player.get())->shortcut[0x2a] = 0;
	player->status->second.item.emplace(0, -1);
	player->status->second.item.emplace(1, -1);
	player->status->second.item.emplace(2, -1);
	player->status->second.item.emplace(3, -1);
	player->status->second.item.emplace(100, 10);
	player->status->second.item.emplace(101, -1);
	player->status->second.item.emplace(102, 10);
	player->status->second.item.emplace(103, 10);
	player->status->second.item.emplace(104, 10);
	player->status->second.item.emplace(105, 10);
	player->status->second.item.emplace(106, 10);
	player->status->second.item.emplace(200, 10);
	player->status->second.item.emplace(201, 10);
	player->status->second.item.emplace(202, 10);
	player->status->second.item.emplace(203, 10);
	player->status->second.item.emplace(300, 10);
	player->status->second.item.emplace(301, 10);
	player->status->second.item.emplace(302, 10);
	player->status->second.item.emplace(303, 10);
	player->status->second.item.emplace(304, 10);
	player->status->second.item.emplace(305, 10);
	player->status->second.item.emplace(400, 10);
	player->status->second.item.emplace(401, 10);
	player->status->second.item.emplace(402, 10);
	player->status->second.item.emplace(403, 10);
	player->status->second.item.emplace(404, 10);

	volume.mute &= 0b11111111;
	volume.bgm = 128;

	BGM::volume = volume.Bgm() ? volume.bgm : 0;

	BGM::play(0);
	ParticleSystem::add<Dust>(20);
	gameState = GameState::title;
}

bool Manager::update()
{
	// タイトル画面処理
	if(gameState == GameState::title)
	{
		--var[0];
		if(Keyboard::push(VK_UP))
		{
			if((--var[1]) < 0)
				var[1] = 2;
			var[0] = 12;
		}
		else if(Keyboard::press(VK_UP) && var[0] <= 0)
		{
			if((--var[1]) < 0)
				var[1] = 4;
			var[0] = 4;
		}
		else if(Keyboard::push(VK_DOWN))
		{
			var[1] = ++var[1] % 3;
			var[0] = 12;
		}
		else if(Keyboard::press(VK_DOWN) && var[0] <= 0)
		{
			var[1] = ++var[1] % 3;
			var[0] = 4;
		}
		if(Keyboard::push(VK_SPACE))
		{
			if(var[1] == 0)
			{
				var[1] = 1;
				BGM::play(1);
				ParticleSystem::reset();
				ParticleSystem::add<Dust>(20);
				gameState = GameState::play;
			}
			else if(var[1] == 1)
			{

			}
			else if(var[1] == 2)
			{
				return false;
			}
		}
	}
	// ゲームメイン画面処理
	else if(gameState == GameState::play)
	{
		// プレイヤーのクールタイム減少
		var[0] = player->status->second.cool = __max(player->status->second.cool - 1, 0);
		// インベントリが開いている場合、そちらに操作を吸わせる
		if(Inventory::active)
		{
			Inventory::controll(static_cast<Player*>(player.get()));
		}
		// どこにも操作を吸わせる先が無かったらメイン操作
		else if(player->status->second.cool <= 0)
		{
			if(Keyboard::push(VK_SHIFT))
			{
				Inventory::active = true;
			}
			else if(Keyboard::press(VK_UP))
			{
				// 前進
				auto i = Field::getIterator(player->pos, player->status->second.speedFront, true);
				if(i == Field::cend())
				{
					player->pos += player->status->second.speedFront;
				}
				else
				{
					if(!(**i) || (*i)->status->second.flag & 1)
					{
						player->pos += player->status->second.speedFront;
						auto j = Field::getIterator(player->pos);
						std::iter_swap(i, j);
					}
					else
					{
						player->pos = (*i)->pos - 1;
					}
					// 接触時イベント
					if(!static_cast<bool>((*i)->status->second.flag & 2))
					{
						(*i)->execute(Action::touch, *player);
						(*i)->status->second.flag |= 2;
					}
				}
				Display::shake.set(0, 8, {4.0f,0.0f});
				player->status->second.cool = 20;	// 仮
			}
			else if(Keyboard::press(VK_DOWN))
			{
				// 後退
				auto i = Field::getIterator(player->pos, -player->status->second.speedBack, true);
				player->pos -= player->status->second.speedBack;
				if(i != Field::cend())
				{
					auto j = Field::getIterator(player->pos);
					std::iter_swap(i, j);
				}
				Display::shake.set(0, 8, {4.0f,0.0f});
				player->status->second.cool = 20;	// 仮
			}
			else
			{
				// アイテムショートカットの実行
				for(BYTE i = 0; i < 64; ++i)
					if(Keyboard::press(0x30 + i))
					{
						int id = static_cast<Player*>(player.get())->shortcut[i];
						if(id != -1)
						{
							auto item = DataBase::item.find(id);
							if(item != DataBase::item.end())
							{
								auto playerItem = player->status->second.item.find(id);
								if(playerItem != player->status->second.item.cend())
								{
									auto target = Field::get(player->pos, item->second.reach);
									if((target.expired() ? Action::execute(item->second.id, *player) : target.lock()->execute(item->second.id, *player)) && playerItem->second != -1)
										if(--playerItem->second == 0)
											player->status->second.item.erase(playerItem);
									player->status->second.cool = 20;	// 仮
								}
							}
						}
					}
			}
		}
		// フィールド上にいるやつの処理
		for(auto i = Field::begin(); i != Field::cend();)
		{
			// 削除
			if((*i)->status->second.flag & 4)
			{
				if((*i)->type == Object::Type::player)
				{
					gameState = GameState::over;
					break;
				}
				i = Field::erase(i);
				continue;
			}
			if((*i)->type == Object::Type::enemy && --(*i)->status->second.cool <= 0)
			{
				for(const auto& j : (*i)->status->second.item)
				{
					if(j.first < DataBase::enemyAction.size())
					{
						auto target = Field::get((*i)->pos, -DataBase::enemyAction[j.first].reach);
						target.expired() ? Action::execute(DataBase::enemyAction[j.first].id, **i) : target.lock()->execute(DataBase::enemyAction[j.first].id, **i);
					}
				}
			}
			++i;
		}
		if(var[0] != player->status->second.cool)
			var[1] = player->status->second.cool;
	}
	else if(gameState == GameState::over)
	{
	}

	TextManager::update();

	// パーティクル更新
	ParticleSystem::update();
	if(!std::uniform_int_distribution{0,16}(mt))
		ParticleSystem::add<Dust>(1);

	// BGM更新処理
	BGM::update();
	// ハンドル管理更新
	HandleManager::update();
	// ディスプレイ振動更新
	Display::shake.update();

	return true;
}

void Manager::draw()
{
	if(gameState == GameState::title)
	{
		ui.DrawRawString(1000, 300, u8"Start", (var[1] == 0) ? 0xffffffff : 0xffa4a4a4, Ref::right);
		ui.DrawRawString(900, 380, u8"Config", (var[1] == 1) ? 0xffffffff : 0xffa4a4a4, Ref::right);
		ui.DrawRawString(800, 460, u8"Quit", (var[1] == 2) ? 0xffffffff : 0xffa4a4a4, Ref::right);
	}
	else if(gameState == GameState::play)
	{
		display.DrawGraph(0, 0, back, true);
		//display.DrawGraph(-100, 20, HandleManager::get(player->status->first->graph, HandleManager::Type::graph), true);

		auto it = Field::cend();
		while(it != Field::begin() && (*--it)->pos > player->pos + static_cast<Player*>(player.get())->searchRange);
		while((*it)->pos > player->pos && (*it)->pos <= player->pos + static_cast<Player*>(player.get())->searchRange)
		{
			display.DrawGraph(0, 0, HandleManager::get((*it)->status->first->graph, HandleManager::Type::graph), true);
			if(it == Field::begin())
				break;
			--it;
		}

		// HUD
		ui.DrawBox(5, 5, {26,400}, 0xff134b13, true);
		ui.DrawBox(5, 5, {26,player->status->second.hp * 400 / player->status->first->hp}, 0xff88af88, true);
		ui.DrawBox(5, 5, {26,400}, 0xff666666, false);
		ui.DrawBox(5, 31, {5,(var[1] - player->status->second.cool) * 400 / var[1]}, 0x968412, true);
		ui.DrawRawString(400, 8, ext::to_u8string(player->status->second.hp), 0xffffffff, Ref::right);
		ui.DrawRawString(420, 8, u8"現在地：", 0xffa4a4a4);
		ui.DrawRawString(495, 8, u8"病院", 0xffa4a4a4);
		ui.DrawRawString(550, 8, u8"(" + ext::to_u8string(player->pos) + u8")", 0xffa4a4a4);
		ui.DrawCircle(10, 50, 6, 0xffa4a4a4, true);
		for(auto i = Field::begin(); i != Field::cend(); ++i)
		{
			if((*i)->type == Object::Type::player || (*i)->pos < player->pos)
				continue;
			else if((*i)->pos > player->pos + static_cast<Player*>(player.get())->searchRange)
				break;
			ui.DrawCircle(10 + ((*i)->pos - player->pos) * 10, 50, 8, 0xffffffff, true);
		}

		Inventory::draw(static_cast<Player*>(player.get()));
	}
	else if(gameState == GameState::over)
	{

	}

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

