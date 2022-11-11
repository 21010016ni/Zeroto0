#include "Manager.hpp"
#include <fstream>
#include <format>
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
#include "Particle.hpp"
#include "Popup.hpp"

Display Manager::display({0,0}, Application::WindowSize, 0);
Display Manager::ui({0,0}, Application::WindowSize, 2);

std::vector<int> Manager::cont = {
	0,1,2,300,301,302,303,304,305
};

extern std::mt19937 engine;

#define DemoStart 1200

bool Manager::save()
{
	std::ofstream ofs("data/save.dat", std::ios::binary);
	if (!ofs.is_open())
		return false;
	unsigned char size = 0;
	ofs.write(reinterpret_cast<const char*>(&(size = static_cast<unsigned char>(player->status->first->item.size()))), 1);
	for (const auto& i : player->status->first->item)
	{
		ofs.write(reinterpret_cast<const char*>(&i.first), 4);
		ofs.write(reinterpret_cast<const char*>(&i.second), 4);
	}
	ofs.close();
	return true;
}

bool Manager::load()
{
	std::ifstream ifs("data/save.dat", std::ios::binary);
	if (!ifs.is_open())
		return false;
	player->status->first->item.clear();
	unsigned char size = 0;
	int buf0 = 0, buf1 = 0;
	ifs.read(reinterpret_cast<char*>(&size), 1);
	for (unsigned char i = 0; i < size; ++i)
	{
		ifs.read(reinterpret_cast<char*>(&buf0), 4);
		ifs.read(reinterpret_cast<char*>(&buf1), 4);
		player->status->first->item.emplace(buf0, buf1);
	}
	ifs.close();
	return true;
}

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

	Effect::load(u8"data/effect/pipo-btleffect001.png", 5, 1, 2, u8"data/se/刀剣・斬る01.mp3"); 
	Effect::load(u8"data/effect/pipo-btleffect002.png", 9, 1, 2, u8"data/se/middle_punch3.mp3");
	Effect::load(u8"data/effect/pipo-btleffect003.png", 5, 1, 2, u8"data/se/打撃2.mp3");
	Effect::load(u8"data/effect/pipo-btleffect004.png", 7, 1, 2, u8"data/se/打撃5.mp3");
	Effect::load(u8"data/effect/pipo-btleffect005.png", 9, 1, 2, u8"data/se/swish1_3.mp3");
	Effect::load(u8"data/effect/pipo-btleffect005_.png", 9, 1, 2,u8"data/se/swish1_3.mp3");
	Effect::load(u8"data/effect/pipo-btleffect006.png", 7, 1, 2, u8"data/se/鞭を振り回す1.mp3");
	Effect::load(u8"data/effect/pipo-btleffect006_.png", 7, 1, 2,u8"data/se/鞭を振り回す1.mp3");
	Effect::load(u8"data/se/パンチの衣擦れ1.mp3");
	Effect::load(u8"data/se/パンチの衣擦れ2.mp3");
	Effect::load(u8"data/se/重いパンチ2.mp3");

	volume.mute &= 0b11111011;
	volume.bgm = 128;
	volume.se = 64;

	BGM::volume = volume.GetBGM();
	Effect::volume = volume.GetSE();

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
		++var[2];
		if (var[2] < DemoStart)
		{
			if (Keyboard::push(VK_UP))
			{
				if ((--var[1]) < 0)
					var[1] = 2;
				var[0] = 12;
			}
			else if (Keyboard::press(VK_UP) && var[0] <= 0)
			{
				if ((--var[1]) < 0)
					var[1] = 4;
				var[0] = 4;
			}
			else if (Keyboard::push(VK_DOWN))
			{
				var[1] = ++var[1] % 3;
				var[0] = 12;
			}
			else if (Keyboard::press(VK_DOWN) && var[0] <= 0)
			{
				var[1] = ++var[1] % 3;
				var[0] = 4;
			}
			if (Keyboard::push(VK_SPACE))
			{
				if (var[1] == 0)
				{
					var[1] = 1;
					// BGM再生
					BGM::play(1);
					// パーティクルリセット
					ParticleSystem::reset();
					ParticleSystem::add<Dust>(20);
					// プレイヤーのステータスリセット
					player = Field::set(new Object(0, &DataBase::player, -1));
					// アイテム読み込み
					if (!load())
					{
						player->status->first->item[0] = -1;
						player->status->first->item[2] = -1;
						player->status->first->item[200] = 6;
						save();
					}
					// アイテム登録
					player->reset(-1);
					// ショートカット
					static_cast<Player*>(player->status->first)->shortcut[0x2a] = 0;
					static_cast<Player*>(player->status->first)->shortcut[0x28] = 2;
					static_cast<Player*>(player->status->first)->shortcut[0x13] = 200;

					// フィールドセット
					Field::set(new Object(1));
					Field::set(new Object(20));
					Field::set(new Object(40));
					Field::set(new Object(50, &(DataBase::enemy.find(200)->second), 0));
					Field::set(new Object(150));

					DataBase::gameFlag = 0;

					gameState = GameState::play;
				}
				else if (var[1] == 1)
				{
					var[0] = 12;
					// 選択位置
					var[1] = 0;
					gameState = GameState::config;
				}
				else if (var[1] == 2)
				{
					return false;
				}
			}
			if (Keyboard::press())
			{
				var[2] = 0;
			}
		}
		else if (Keyboard::press())
		{
			PauseMovieToGraph(Handle::get(u8"data/movie/demo.mp4", Handle::Type::graph));
			SeekMovieToGraph(Handle::get(u8"data/movie/demo.mp4", Handle::Type::graph), 0);
			var[2] = 0;
			var[0] = 0;
		}
	}
	else if(gameState == GameState::config)
	{
		--var[0];
		if(Keyboard::push(VK_SHIFT))
		{
			var[0] = 12;
			// 選択位置
			var[1] = 1;
			gameState = GameState::title;
		}
		else if(Keyboard::push(VK_UP))
		{
			if((--var[1]) < 0)
				var[1] = 4 - 1;
			var[0] = 12;
		}
		else if(Keyboard::press(VK_UP) && var[0] <= 0)
		{
			if((--var[1]) < 0)
				var[1] = 4 - 1;
			var[0] = 4;
		}
		else if(Keyboard::push(VK_DOWN))
		{
			var[1] = ++var[1] % 4;
			var[0] = 12;
		}
		else if(Keyboard::press(VK_DOWN) && var[0] <= 0)
		{
			var[1] = ++var[1] % 4;
			var[0] = 4;
		}
		else if(Keyboard::push(VK_LEFT))
		{
			switch(var[1])
			{
			case 0:
				volume.master = __max(volume.master - 0.01f, 0.0f);
				break;
			case 1:
				volume.bgm = __max(static_cast<short>(volume.bgm) - 1, 0);
				break;
			case 2:
				volume.se = __max(static_cast<short>(volume.se) - 1, 0);
				break;
			case 3:
				displayControll ^= true;
				break;
			}
			var[0] = 12;
		}
		else if(Keyboard::press(VK_LEFT) && var[0] <= 0)
		{
			switch(var[1])
			{
			case 0:
				volume.master = __max(volume.master + 0.01f * __max((var[0] / 6), -5), 0.0f);
				break;
			case 1:
				volume.bgm = __max(static_cast<short>(volume.bgm) + 1 * __max((var[0] / 6), -10), 0);
				break;
			case 2:
				volume.se = __max(static_cast<short>(volume.se) + 1 * __max((var[0] / 6), -10), 0);
				break;
			case 3:
				displayControll ^= true;
				var[0] = 4;
				break;
			}
		}
		else if(Keyboard::push(VK_RIGHT))
		{
			switch(var[1])
			{
			case 0:
				volume.master = __min(volume.master + 0.01f, 1.0f);
				break;
			case 1:
				volume.bgm = __min(static_cast<short>(volume.bgm) + 1, 255);
				break;
			case 2:
				volume.se = __min(static_cast<short>(volume.se) + 1, 255);
				break;
			case 3:
				displayControll ^= true;
				break;
			}
			var[0] = 12;
		}
		else if(Keyboard::press(VK_RIGHT) && var[0] <= 0)
		{
			switch(var[1])
			{
			case 0:
				volume.master = __min(volume.master - 0.01f * __max((var[0] / 6), -5), 1.0f);
				break;
			case 1:
				volume.bgm = __min(static_cast<short>(volume.bgm) - 1 * __max((var[0] / 6), -10), 255);
				break;
			case 2:
				volume.se = __min(static_cast<short>(volume.se) - 1 * __max((var[0] / 6), -10), 255);
				break;
			case 3:
				displayControll ^= true;
				var[0] = 4;
				break;
			}
		}
		else if(Keyboard::push(VK_SPACE))
		{
			switch(var[1])
			{
			case 0:
				volume.mute ^= 0x01;
				break;
			case 1:
				volume.mute ^= 0x02;
				break;
			case 2:
				volume.mute ^= 0x04;
				break;
			case 3:
				displayControll ^= true;
				break;
			}
		}
		BGM::ChangeVolume(volume.GetBGM());
		Effect::volume = volume.GetSE();
	}
	// ゲームメイン画面処理
	else if(gameState == GameState::play)
	{
		// プレイヤーのクールタイム減少
		var[0] = player->status->second.cool = __max(player->status->second.cool - 1, 0);

		// プレイヤーのエフェクト減少、一部の効果発揮
		if(++var[2] >= 60)
		{
			var[2] = 0;
			for(auto i = player->status->second.state.begin(); i != player->status->second.state.cend();)
			{
				if(i->first == Status::State::poison)
				{
					player->status->second.hp -= 1;
				}
				else if(i->first == Status::State::saturation)
				{
					player->status->second.hp += 1;
				}

				if(--i->second <= 0)
				{
					i = player->status->second.state.erase(i);
					continue;
				}
				++i;
			}
		}

		// インベントリが開いている場合、そちらに操作を吸わせる
		if(Inventory::active)
		{
			Inventory::controll(player);
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
				auto i = Field::getIterator(player->pos, player->status->second.speedFront, true, true);
				if(i == Field::cend())
				{
					player->pos += player->status->second.speedFront;
				}
				else
				{
					// 接触時イベント
					if (!(**i) || !static_cast<bool>((*i)->status->second.flag & 2))
					{
						(*i)->execute(Action::touch, *player);
						if (**i)
							(*i)->status->second.flag |= 2;
					}
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
				}
				// 敵追加
				if (!std::uniform_int_distribution{ 0,3 }(engine))
				{
					Field::set(new Object(player->pos + player->status->first->range, &(DataBase::enemy.find(__min(player->pos / 60, 4))->second), 4));
				}

				Effect::set(8, 0, 0, Effect::Pos::leftup);

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

				Effect::set(9, 0, 0, Effect::Pos::leftup);

				Display::shake.set(0, 8, {4.0f,0.0f});
				player->status->second.cool = 40;	// 仮
			}
			else if(Keyboard::push(VK_F12))
			{
				// タイトルに戻る
				TextManager::reset();
				// フィールドのリセット
				Field::reset();
				// プレイヤーの所持アイテム引継ぎ登録
				for(const auto& i : cont)
				{
					auto itemBuf = player->status->second.item.find(i);
					if(itemBuf != player->status->second.item.end())
						player->status->first->item[i] = itemBuf->second;
				}
				save();
				// ショートカット
				for(auto& i : static_cast<Player*>(player->status->first)->shortcut)
					i = -1;
				// パーティクル
				ParticleSystem::reset();
				ParticleSystem::add<Dust>(20);
				// インベントリ
				Inventory::active = false;
				// 振動のリセット
				Display::shake.reset();
				// BGM
				BGM::play(0);
				// 選択位置
				var[0] = 0;
				var[1] = 0;
				gameState = GameState::title;
			}
			else
			{
				// アイテムショートカットの実行
				for(BYTE i = 0; i < 64; ++i)
					if(Keyboard::press(0x30 + i))
					{
						int id = static_cast<Player*>(player->status->first)->shortcut[i];
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
								}
							}
						}
					}
			}
		}
		player->status->first->hp = __max(player->status->first->hp, player->status->second.hp);

		// フィールド上にいるやつの処理
		for(auto i = Field::begin(); i != Field::cend();)
		{
			// 削除
			if ((*i)->pos < player->pos || ((**i) && (*i)->status->second.flag & 4))
			{
				if ((*i)->type() == Status::Type::player)
				{
					gameState = GameState::over;
					break;
				}
				if (**i)
					player->status->second.AddItem((*i)->status->second.item);
				i = Field::erase(i);
				continue;
			}
			if ((*i)->type() == Status::Type::enemy && --(*i)->status->second.cool <= 0 && static_cast<Enemy*>((*i)->status->first)->action != -1)
			{
				auto target = Field::get((*i)->pos, -(*i)->status->first->range);
				target.expired() ? Action::execute(static_cast<Enemy*>((*i)->status->first)->action, **i) : target.lock()->execute(static_cast<Enemy*>((*i)->status->first)->action, **i);
			}
			++i;
		}

		if(var[0] != player->status->second.cool)
			var[1] = player->status->second.cool;
	}
	else if(gameState == GameState::over)
	{
		if (Keyboard::push())
		{
			TextManager::reset();
			// フィールドのリセット
			Field::reset();
			// プレイヤーの所持アイテム引継ぎ登録
			for (const auto& i : cont)
			{
				auto itemBuf = player->status->second.item.find(i);
				if (itemBuf != player->status->second.item.end())
					player->status->first->item[i] = itemBuf->second;
			}
			save();
			// ショートカット
			for (auto& i : static_cast<Player*>(player->status->first)->shortcut)
				i = -1;
			// パーティクル
			ParticleSystem::reset();
			ParticleSystem::add<Dust>(20);
			// インベントリ
			Inventory::active = false;
			// 振動のリセット
			Display::shake.reset();
			// BGM
			BGM::play(0);
			// 選択位置
			var[0] = 0;
			var[1] = 0;
			gameState = GameState::title;
		}
	}

	TextManager::update();
	Popup::update();

	// パーティクル更新
	ParticleSystem::update();
	if(!std::uniform_int_distribution{0,16}(engine))
		ParticleSystem::add<Dust>(1);

	// BGM更新処理
	BGM::update();
	// ハンドル管理更新
	Handle::update();
	// ディスプレイ振動更新
	Display::shake.update();

	return true;
}

void Manager::draw()
{
	if(gameState == GameState::title)
	{
		if (var[2] > DemoStart + 64)
		{
			var[0] = static_cast<int>(std::sin((var[2] % 180) * 3.1415927f / 180) * 240) + 64;
			var[0] = __min(var[0], 255);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, (var[2] - DemoStart - 64) * 4);
			if(GetMovieStateToGraph(Handle::get(u8"data/movie/demo.mp4", Handle::Type::graph)) == 0)
			{
				SeekMovieToGraph(Handle::get(u8"data/movie/demo.mp4", Handle::Type::graph), 0);
				PlayMovieToGraph(Handle::get(u8"data/movie/demo.mp4", Handle::Type::graph));
			}
			DrawExtendGraph(0, 0, 1024, 600, Handle::get(u8"data/movie/demo.mp4", Handle::Type::graph), false);
			SetDrawBlendMode(DX_BLENDMODE_MUL, 64);
			ui.DrawBox(0, 0, {600,1024}, 0xff888888, true);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, (var[2] - DemoStart - 64) * 4);
			ui.DrawRawString(512, 300, u8"Demo mode", GetColor(var[0], var[0], var[0]), Ref::center | Ref::middle);
		}
		else
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - (var[2] - DemoStart) * 4);
			ui.DrawGraph(100, 100, (std::uniform_int_distribution{ 0,180 }(engine)) ? Handle::get(u8"data/picture/title.png",Handle::Type::graph) : Handle::get(u8"data/picture/titleb.png", Handle::Type::graph), true);
			ui.DrawRawString(920, 300, u8"Start", (var[1] == 0) ? 0xffffffff : 0xffa4a4a4, Ref::right);
			ui.DrawRawString(860, 380, u8"Config", (var[1] == 1) ? 0xffffffff : 0xffa4a4a4, Ref::right);
			ui.DrawRawString(800, 460, u8"Quit", (var[1] == 2) ? 0xffffffff : 0xffa4a4a4, Ref::right);
			if(displayControll)
				ui.DrawRawString(10, 590, u8"Spaceで決定 / ↑↓で選択", 0xffa4a4a4, Ref::under);
		}
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	else if(gameState == GameState::config)
	{
		ui.DrawRawString(800, 300, u8"Master Volume", (var[1] == 0) ? 0xffffffff : 0xffa4a4a4, Ref::right);
		ui.DrawRawString(845, 300, ext::convert(std::format("{:.2f}", volume.master)), (var[1] == 0) ? 0xffffffff : 0xffa4a4a4, Ref::right);
		if((volume.mute & 0x01) == 0)
			ui.DrawRawString(850, 300, u8"(mute)", (var[1] == 0) ? 0xffffffff : 0xffa4a4a4);
		ui.DrawRawString(800, 330, u8"BGM Volume", (var[1] == 1) ? 0xffffffff : 0xffa4a4a4, Ref::right);
		ui.DrawRawString(845, 330, ext::to_u8string(volume.bgm), (var[1] == 1) ? 0xffffffff : 0xffa4a4a4, Ref::right);
		if((volume.mute & 0x02) == 0)
			ui.DrawRawString(850, 330, u8"(mute)", (var[1] == 1) ? 0xffffffff : 0xffa4a4a4);
		ui.DrawRawString(800, 360, u8"SE Volume", (var[1] == 2) ? 0xffffffff : 0xffa4a4a4, Ref::right);
		ui.DrawRawString(845, 360, ext::to_u8string(volume.se), (var[1] == 2) ? 0xffffffff : 0xffa4a4a4, Ref::right);
		if((volume.mute & 0x04) == 0)
			ui.DrawRawString(850, 360, u8"(mute)", (var[1] == 2) ? 0xffffffff : 0xffa4a4a4);
		ui.DrawRawString(800, 390, u8"操作説明の表示", (var[1] == 3) ? 0xffffffff : 0xffa4a4a4, Ref::right);
		ui.DrawRawString(845, 390, displayControll ? u8"有効" : u8"無効", (var[1] == 3) ? 0xffffffff : 0xffa4a4a4, Ref::right);
		if(displayControll)
			ui.DrawRawString(10, 590, u8"←→Spaceで変更 / ↑↓で選択 / Shiftでタイトルへ", 0xffa4a4a4, Ref::under);
	}
	else if(gameState == GameState::play)
	{
		display.DrawGraph(0, 0, back, true);
		//display.DrawGraph(-100, 20, HandleManager::get(player->status->first->graph, HandleManager::Type::graph), true);

		auto it = Field::cend();
		while(it != Field::begin() && (*--it)->pos > player->pos + player->status->first->range);
		while((*it)->pos > player->pos && (*it)->pos <= player->pos + player->status->first->range)
		{
			if (**it)
			{
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, (player->status->first->range - (*it)->pos + player->pos + 1) * 255 / player->status->first->range);
				display.DrawGraph(1024, 600, Handle::get((*it)->status->first->graph, Handle::Type::graph), true, Ref::right | Ref::under);
			}
			if(it == Field::begin())
				break;
			--it;
		}

		// HUD
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		ui.DrawBox(5, 5, {26,400}, 0xff134b13, true);
		ui.DrawBox(5, 5, { 26,__min(player->status->second.hp,player->status->first->hp) * 400 / player->status->first->hp }, 0xff88af88, true);
		ui.DrawBox(5, 5, {26,400}, 0xff666666, false);
		ui.DrawBox(5, 31, {5,(var[1] - player->status->second.cool) * 400 / var[1]}, 0x968412, true);
		ui.DrawRawString(400, 8, ext::to_u8string(player->status->second.hp), 0xffffffff, Ref::right);
		ui.DrawRawString(420, 8, u8"現在地：", 0xffa4a4a4);
		ui.DrawRawString(495, 8, u8"病院", 0xffa4a4a4);
		ui.DrawRawString(550, 8, u8"(" + ext::to_u8string(player->pos) + u8")", 0xffa4a4a4);
		var[0] = 5;
		for(const auto& i : player->status->second.state)
		{
			int id = -1;
			switch(i.first)
			{
			case Status::State::poison:
				id = 0x623;
				break;
			case Status::State::saturation:
				id = 0x625;
				break;
			case Status::State::bright:
				id = 0x630;
				break;
			case Status::State::arousal:
				id = 0x651;
				break;
			}
			ui.DrawIcon(var[0], 36, id);
			var[0] += 24;
		}
		for(int i = 0; i < player->status->first->range; ++i)
		{
			ui.DrawBox(8 + i * 9, 65, {19,9}, 0xffa4a4a4, false);
		}
		for(auto i = Field::begin(); i != Field::cend(); ++i)
		{
			if((*i)->pos < player->pos)
				continue;
			if((*i)->pos > player->pos + player->status->first->range)
				break;
			if((*i)->type() == Status::Type::player)
				ui.DrawCircle(12 + ((*i)->pos - player->pos) * 9, 74, 6, 0xffa4a4a4, true);
			else if((*i)->type() == Status::Type::enemy)
				ui.DrawCircle(12 + ((*i)->pos - player->pos) * 9, 74, 7, 0xffff0000, true);
		}

		if(displayControll)
		{
			if(Inventory::active)
			{
				ui.DrawRawString(10, 570, u8"Spaceで使用 / ↑↓←→で選択 / Shiftで閉じる", 0xffa4a4a4, Ref::under);
				ui.DrawRawString(10, 590, u8"Ctrl + 英数字キーでショートカット登録", 0xffa4a4a4, Ref::under);
			}
			else
			{
				ui.DrawRawString(10, 570, u8"↑↓で移動 / Shiftでメニュー", 0xffa4a4a4, Ref::under);
				ui.DrawRawString(10, 590, u8"英数字キーでショートカット実行", 0xffa4a4a4, Ref::under);
			}
		}
		//for (auto i = Field::begin(); i != Field::cend(); ++i)
		//{
		//	printfDx("%d : ", (*i)->pos);
		//	switch ((*i)->type())
		//	{
		//	case Status::Type::undefined:
		//		printfDx("undefined");
		//		break;
		//	case Status::Type::player:
		//		printfDx("player");
		//		break;
		//	case Status::Type::enemy:
		//		printfDx("enemy");
		//		break;
		//	case Status::Type::shop:
		//		printfDx("shop");
		//		break;
		//	case Status::Type::ev:
		//		printfDx("event");
		//		break;
		//	}
		//	printfDx("\n");
		//}

		Popup::draw();

		Inventory::draw(player);
	}
	else if(gameState == GameState::over)
	{
		display.DrawBox(0, 0, { 600,1024 }, 0xff880000, true);
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

