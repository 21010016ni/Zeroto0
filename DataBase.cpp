#include "DataBase.hpp"
#include <fstream>
#include "Object.hpp"
#include "Text.hpp"
#include <boost/algorithm/string.hpp>
#include "convert_string.hpp"

std::vector<Action> DataBase::action =
{
	{{
		{Action::Key::item_use + 100,[](Object& user, Object& target) {
			target.status->second.flag |= 1;
			TextManager::player.set(u8R"(\b�����g���Ĕ����J�����B\w9\e)");
			return true;
		}},
	}},
	{{
		{Action::Key::touch,[](Object& user, Object& target) {
			target.status->second.flag |= 4;
			TextManager::player.set(u8R"(\b��������̃C�x���g\w9\e)");
			return true;
		}},
	}},
};

std::map<int, Enemy> DataBase::enemy =
{
	// ���O,HP,�U����,�O���ړ����x,����ړ����x,�˒�,�摜,�t���O,�����A�C�e��,�A�N�V�����ԍ�,���A�N�V�������̂ւ̃|�C���^
	{0,Enemy(u8"���ق����", 10, 1, 2, 0, 4, u8"data/picture/flower3868.png", 0,{{300,-1},{301,-1},{302,-1},},0x2000,nullptr)},
	{100,Enemy(u8"��", 200, 0, 0, 0, 0, u8"data/picture/door.png", 1,{},-1,&DataBase::action[0])},
	{200,Enemy(u8"��", 50, 0, 0, 0, 0, u8"data/picture/door.png", 1,{},-1,&DataBase::action[0])},
	{201,Enemy(u8"�C�x���g", 0, 0, 0, 0, 0, u8"data/picture/door.png", 1,{},-1,&DataBase::action[0])},
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

