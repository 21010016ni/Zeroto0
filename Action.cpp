#include "Action.hpp"
#include "Field.hpp"

#include "Text.hpp"
#include <iostream>

bool Action::execute(int id, Object& user, Object& target)
{
	auto i = action.find(id);
	if (i == action.end())
		if ((i = commonActionDouble.find(id)) == commonActionDouble.end())
			return execute(id, user);
	return i->second(user, target);
}

bool Action::execute(int id, Object& user)
{
	auto i = commonActionSingle.find(id);
	if (i == commonActionSingle.end())
	{
		// エラー処理
		return false;
	}
	return i->second(user);
}

/*
* 原則、アクションIDはアイテムIDと同じ値を設定する
* ビット演算の可能性を残すために、16進数で設定
* +0000 ~ +0fff	アイテムを相手に使用
* +1000 ~ +1fff	アイテムを自身に使用
* -0001			接触時
* -0002			倒されたとき
* -0003
*/

enum Key :int
{
	item_use = 0x0000,
	item_self = 0x1000,
	touch = -1,
	killed = -2,
	enemy_action = -0x1000,
};

std::unordered_map<int, Action::ValueSingle> Action::commonActionSingle =
{
	{item_use + 0,[](Object&) {
		TextManager::player.set(u8R"(\bアクション0を単独で実行\w9\e)");
		return true;
	}},
	{item_use + 1,[](Object&) {
		TextManager::player.set(u8R"(\bアクション1を単独で実行\w9\e)");
		return true;
	}},
	{enemy_action - 0,[](Object& u) {
		// 前進
		auto i = Field::getIterator(u.pos, -2, true);
		if(i == Field::cend())
		{
			u.pos -= 2;
		}
		else
		{
			if(!(**i) || (*i)->status->second.flag & 1)
			{
				u.pos -= 2;
				auto j = Field::getIterator(u.pos);
				std::iter_swap(i, j);
			}
			else
			{
				u.pos = (*i)->pos + 1;
			}
			// ここに接触時イベント
		}
		u.status->second.cool = 120;	// 仮

		TextManager::player.set(u8R"(\b敵が移動した\w9\e)");
		return true;
	}},
};

std::unordered_map<int, Action::ValueDouble> Action::commonActionDouble =
{
	{item_use + 0,[](Object&,Object&) {
		TextManager::partner.set(u8R"(\bアクション0を対象を取って実行\w9\e)");
		return true;
	}},
	{item_use + 1,[](Object&,Object&) {
		TextManager::partner.set(u8R"(\bアクション1を対象を取って実行\w9\e)");
		return true;
	}},
	{enemy_action - 0,[](Object& u,Object& t) {
		// 攻撃
		t.status->second.hp -= u.status->second.atk;
		u.status->second.cool = 120;	// 仮

		TextManager::player.set(u8R"(\b敵が攻撃した\w9\e)");
		return true;
	}},
};

