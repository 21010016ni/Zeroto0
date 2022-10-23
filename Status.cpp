#include "Status.hpp"
#include "DataBase.hpp"

StatusInst::StatusInst(const Status& t)
{
	hp = t.hp;
	atk = t.atk;
	speedFront = t.speedFront;
	speedBack = t.speedBack;
	cool = 0;
	flag = t.flag;
	item = t.item;
}

void StatusInst::AddItem(const std::map<int, int>& t)
{
	for(const auto& i : t)
	{
		if(item[i.first] != -1)
		{
			item[i.first] += i.second;
		}
	}
}
