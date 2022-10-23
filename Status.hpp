#pragma once
#include <string>
#include <map>
#include "Text.hpp"

class Status
{
public:
	int hp;
	int atk;
	int speedFront;
	int speedBack;
	std::u8string graph;

	unsigned char flag;	// 最下位ビットは通行可不可の判定に用いる　それ以外は自由

	Status(int hp, int atk, int speedFront, int speedBack, const char8_t* graph, unsigned char flag) :hp(hp), atk(atk), speedFront(speedFront), speedBack(speedBack), graph(graph), flag(flag) {}
	
	std::map<int, int> item;	// 敵の場合ドロップアイテム、プレイヤーの場合引継ぎアイテム
};

class StatusInst
{
public:
	int hp;
	int atk;
	int cool;
	int speedFront;
	int speedBack;

	unsigned char flag;

	std::map<int, int> item;	// 所持アイテム　敵はこれをStatusからコピーして受け取り、ドロップする

	StatusInst(const Status& t);

	void AddItem(const std::map<int, int>& t);
};