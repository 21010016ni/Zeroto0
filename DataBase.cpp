#include "DataBase.hpp"
#include <fstream>
#include "Object.hpp"
#include "Text.hpp"
#include <boost/algorithm/string.hpp>
#include "convert_string.hpp"

std::vector<Action> DataBase::action =
{
	{{
		{Action::Key::item_use + 100,[](Object& u, Object& t) {
			//target.status->second.flag |= 1;
			t.status->second.flag |= 4;
			TextManager::player.set(u8R"(\b鍵を使って扉を開いた。\w9\e)");
			u.status->second.cool = 60;
			return true;
		}},
		{Action::Key::item_use + 0,[](Object& u, Object& t) {
			TextManager::player.set((u8R"(\b扉だ。鍵がかかっていて開かない。\nあと)" + ext::to_u8string(t.status->second.hp / u.status->second.atk) + u8R"(回も殴れば開きそうだ。\w9\e)").c_str());
			u.status->second.cool = 20;
			return true;
		}},
	}},
	{{
		{Action::Key::item_use + 0,[](Object& u, Object& t) {
			TextManager::player.set((u8R"(\b怪物。\w4)" + t.status->first->name + u8R"(。\w9\e)").c_str());
			u.status->second.cool = 20;
			return true;
		}},
	}},
	{{
		{Action::Key::item_use + 100,[](Object& u, Object& t) {
			//target.status->second.flag |= 1;
			t.status->second.flag |= 4;
			TextManager::player.set(u8R"(\b鍵を使って宝箱を開いた。\w9\e)");
			u.status->second.AddItem(t.status->second.item);
			u.status->second.cool = 60;
			return true;
		}},
		{Action::Key::item_use + 0,[](Object& u, Object& t) {
			TextManager::player.set((u8R"(\b宝箱だ。鍵がかかっていて開かない。\nあと)" + ext::to_u8string(t.status->second.hp / u.status->second.atk) + u8R"(回も殴れば開きそうだ。\w9\e)").c_str());
			u.status->second.cool = 20;
			return true;
		}},
	}},
};

std::map<int, Enemy> DataBase::enemy =
{
	// 名前,HP,攻撃力,前方移動速度,後方移動速度,射程,画像,フラグ,所持アイテム,アクション番号,リアクション実体へのポインタ
	{0,Enemy(u8"沈黙する花", 10, 1, 2, 0, 4, u8"data/picture/flower3868.png", 0,{{100,1},{201,1},{201,1},{202,1},{202,1},{203,1},{203,1}},0x2000,&DataBase::action[1])},
	{1,Enemy(u8"追想する花", 30, 4, 3, 0, 4, u8"data/picture/flower0497.png", 0,{{201,1},{202,1},{203,1},{102,1},{400,1},{401,1},{404,1}},0x2000,&DataBase::action[1])},
	{2,Enemy(u8"否定する花", 60, 7, 3, 0, 4, u8"data/picture/flower1719.png", 0,{{100,1},{103,1},{103,1},{104,1},{204,1},{206,1},{206,1},{400,1},{401,1}},0x2000,&DataBase::action[1])},
	{3,Enemy(u8"悔恨する花", 100, 10, 4, 0, 4, u8"data/picture/flower3914.png", 0,{{101,-1},{106,1},{204,1},{204,1},{205,1},{205,1},{402,1},{403,1}},0x2000,&DataBase::action[1])},
	{4,Enemy(u8"哄笑する花", 150, 14, 2, 0, 4, u8"data/picture/flower1720.png", 0,{{105,1},{204,2},{204,1},{207,1},{401,1},{402,1},{404,1}},0x2000,&DataBase::action[1]) },
	{100,Enemy(u8"扉", 200, 0, 0, 0, 0, u8"data/picture/door.png", 0,{},-1,&DataBase::action[0])},
	{200,Enemy(u8"扉", 50, 0, 0, 0, 0, u8"data/picture/door.png", 0,{},-1,&DataBase::action[0])},
	{101,Enemy(u8"宝箱",80,0,0,0,0,u8"data/picture/box.png",1,{{102,1},{103,1},{104,1},{105,1},{106,1},{201,5},{202,5},{203,4}},-1,&DataBase::action[2])},
	{102,Enemy(u8"宝箱",80,0,0,0,0,u8"data/picture/box.png",1,{{102,1},{103,1},{104,1},{105,1},{106,1},{204,5},{205,5},{206,4}},-1,&DataBase::action[2])},
};

void DataBase::LoadItem(const char* FileName)
{
	std::ifstream ifs(FileName);
	if (!ifs.is_open())
		throw;
	std::string line;
	std::vector<std::string> elem;
	while (std::getline(ifs, line))
	{
		if (line.empty() || line.front() == '#')
			continue;
		boost::split(elem, line, boost::is_any_of(","));
		if (elem.size() != 6)
			continue;
		item.emplace(std::stoi(elem[0]), Item(ext::tochar(elem[1]), ext::tochar(elem[2]), std::stoi(elem[3], nullptr, 16), std::stoi(elem[4]), std::stoi(elem[5])));
	}
}

Player DataBase::player(u8"ゼロ", 20, 10, 7, 5, 30, u8"data/picture/sibyl.png", 0, {}, 0);

