#pragma once
#include <string>
#include <map>
#include <vector>
#include "Action.hpp"

class Status
{
public:
	enum class State :char
	{
		poison,
		saturation,
		bright,
		arousal,
	};
	enum class Type :char
	{
		undefined,
		player,
		enemy,
		shop,
		ev,
	} type;

	std::u8string name;
	int hp;
	int atk;
	int speedFront;
	int speedBack;
	int range;

	std::u8string graph;

	unsigned char flag;	// 最下位ビットは通行可不可の判定に用いる　それ以外は自由

	std::map<int, int> item;	// 敵の場合ドロップアイテム、プレイヤーの場合引継ぎアイテム

	Action* action;

	Status(const char8_t* name, int hp, int atk, int speedFront, int speedBack, int range, const char8_t* graph, unsigned char flag, std::map<int, int> item, Action* action) :type(Type::undefined), name(name), hp(hp), atk(atk), speedFront(speedFront), speedBack(speedBack), range(range), graph(graph), flag(flag), item(item), action(action)
	{
	}
};

class Player :public Status
{
public:
	int partner;
	std::vector<int> shortcut;

	Player(const char8_t* name, int hp, int atk, int speedFront, int speedBack, int range, const char8_t* graph, unsigned char flag, std::map<int, int> item, int partner) :Status(name, hp, atk, speedFront, speedBack, range, graph, flag, item, {}), partner(partner)
	{
		type = Type::player;
		shortcut.resize(64, -1);
	}
};

class Enemy :public Status
{
public:
	int action;

	Enemy(const char8_t* name, int hp, int atk, int speedFront, int speedBack, int range, const char8_t* graph, unsigned char flag, std::map<int, int> item, int action, Action* reaction) :Status(name, hp, atk, speedFront, speedBack, range, graph, flag, item, reaction), action(action)
	{
		type = Type::enemy;
	}
};

class Shop :public Status
{
public:
	Shop(const char8_t* name, int hp, int atk, int speedFront, int speedBack, int range, const char8_t* graph, unsigned char flag, std::map<int, int> item, int action, Action* reaction) :Status(name, hp, atk, speedFront, speedBack, range, graph, flag, item, reaction)
	{
		type = Type::shop;
	}
};

class StatusInst
{
public:
	int hp;
	int atk;
	int cool;
	int speedFront;
	int speedBack;

	unsigned char flag;	// -,-,-,-, -,次タイミングで削除,接触済み,通行不可

	std::map<int, int> item;	// 所持アイテム　敵はこれをStatusからコピーして受け取り、ドロップする
	std::map<Status::State, int> state;	// 現在状態、secondは時間

	StatusInst(const Status& t, int num);

	void AddItem(const std::map<int, int>& t);
};

