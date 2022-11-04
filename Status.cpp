#include "Status.hpp"
#include "Popup.hpp"
#include "DataBase.hpp"
#include "convert_string.hpp"

extern std::mt19937 engine;

StatusInst::StatusInst(const Status& t, int num)
{
	hp = t.hp;
	atk = t.atk;
	speedFront = t.speedFront;
	speedBack = t.speedBack;
	cool = 0;
	flag = t.flag;
	if (num != -1)
	{
		std::vector<std::pair<int, int>> buf;
		for (const auto& i : t.item)
			buf.emplace_back(i.first, i.second);
		std::shuffle(buf.begin(), buf.end(), engine);
		for (int i = 0; !buf.empty() && i < num; ++i)
		{
			item[buf.back().first] += buf.back().second;
			//item.emplace(buf.back().first, buf.back().second);
			buf.pop_back();
		}
	}
	else
	{
		item = t.item;
	}
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
