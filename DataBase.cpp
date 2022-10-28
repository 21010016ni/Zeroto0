#include "DataBase.hpp"
#include <fstream>
#include "Object.hpp"
#include "Text.hpp"
#include <boost/algorithm/string.hpp>
#include "convert_string.hpp"

std::vector<Action> DataBase::action =
{
	Action({
		{Action::Key::item_use+100,[](Object& user, Object& target) {
			target.status->second.flag |= 1;
			TextManager::player.set(u8R"(\b鍵を使って扉を開いた。\w9\e)");
			return true;
		}},
		}),
};

std::vector<Enemy> DataBase::enemy =
{
	// 名前,HP,攻撃力,前方移動速度,後方移動速度,射程,画像,フラグ,所持アイテム,アクション番号,リアクション実体へのポインタ
	{u8"沈黙する花", 10, 1, 2, 0, 4, u8"data/picture/flower3868.png", 0,{{300,-1},{301,-1},{302,-1},},0x2000,&DataBase::action[0]},
	{u8"扉", 200, 0, 0, 0, 0, u8"data/picture/door.png", 0,{},-1,&DataBase::action[0]},
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

