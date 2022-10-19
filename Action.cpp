#include "Action.hpp"
#include "Field.hpp"

#include <iostream>

bool Action::execute(int id, Object& user, Object& target)
{
	auto i = action.find(id);
	if(i == action.end())
		if((i = commonActionDouble.find(id)) == commonActionDouble.end())
			return execute(id, user);
	return i->second(user, target);
}

bool Action::execute(int id, Object& user)
{
	auto i = commonActionSingle.find(id);
	if(i == commonActionSingle.end())
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
};

std::unordered_map<int, Action::ValueSingle> Action::commonActionSingle =
{
	{item_use + 0,[](Object&) { std::cout << "アクション0を単独で実行" << std::endl; return true; }},
	{item_use + 1,[](Object&) { std::cout << "アクション1を単独で実行" << std::endl; return true; }},
};

std::unordered_map<int, Action::ValueDouble> Action::commonActionDouble =
{
	{item_use + 0,[](Object&,Object&) { std::cout << "アクション0を対象を取って実行" << std::endl; return true; }},
	{item_use + 1,[](Object&,Object&) { std::cout << "アクション1を対象を取って実行" << std::endl; return true; }},
};

