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

	unsigned char flag;	// �ŉ��ʃr�b�g�͒ʍs�s�̔���ɗp����@����ȊO�͎��R

	std::map<int, int> item;	// �G�̏ꍇ�h���b�v�A�C�e���A�v���C���[�̏ꍇ���p���A�C�e��

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

	unsigned char flag;	// -,-,-,-, -,���^�C�~���O�ō폜,�ڐG�ς�,�ʍs�s��

	std::map<int, int> item;	// �����A�C�e���@�G�͂����Status����R�s�[���Ď󂯎��A�h���b�v����
	std::map<Status::State, int> state;	// ���ݏ�ԁAsecond�͎���

	StatusInst(const Status& t, int num);

	void AddItem(const std::map<int, int>& t);
};

