#include "Status.hpp"
#include "Popup.hpp"
#include "DataBase.hpp"
#include "convert_string.hpp"

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
	int v = 180;
	for(const auto& i : t)
	{
		if(item[i.first] != -1)
		{
			item[i.first] += i.second;
			if (i.second == -1)
				Popup::push(v, DataBase::item.find(i.first)->second.name, u8"‚ðŠl“¾");
			else
				Popup::push(v, DataBase::item.find(i.first)->second.name, u8"+" + ext::to_u8string(i.second));
			v += 20;
		}
	}
}
