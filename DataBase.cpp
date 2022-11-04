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
			//target.status->second.flag |= 1;
			target.status->second.flag |= 4;
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
	{0,Enemy(u8"���ق����", 10, 1, 2, 0, 4, u8"data/picture/flower3868.png", 0,{{100,1},{201,1},{201,1},{202,1},{202,1},{203,1},{203,1}},0x2000,nullptr)},
	{1,Enemy(u8"�Ǒz�����", 30, 4, 3, 0, 4, u8"data/picture/flower0497.png", 0,{{201,1},{202,1},{203,1},{102,1},{400,1},{401,1},{404,1}},0x2000,nullptr)},
	{2,Enemy(u8"�ے肷���", 60, 7, 3, 0, 4, u8"data/picture/flower1719.png", 0,{{100,1},{103,1},{103,1},{104,1},{204,1},{206,1},{206,1},{400,1},{401,1}},0x2000,nullptr)},
	{3,Enemy(u8"���������", 100, 10, 4, 0, 4, u8"data/picture/flower3914.png", 0,{{101,-1},{106,1},{204,1},{204,1},{205,1},{205,1},{402,1},{403,1}},0x2000,nullptr)},
	{4,Enemy(u8"���΂����", 150, 14, 2, 0, 4, u8"data/picture/flower1720.png", 0,{{105,1},{204,2},{204,1},{207,1},{401,1},{402,1},{404,1}},0x2000,nullptr) },
	{100,Enemy(u8"��", 200, 0, 0, 0, 0, u8"data/picture/door.png", 0,{},-1,&DataBase::action[0])},
	{200,Enemy(u8"��", 50, 0, 0, 0, 0, u8"data/picture/door.png", 0,{},-1,&DataBase::action[0])},
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

Player DataBase::player(u8"�[��", 20, 10, 7, 5, 30, u8"data/picture/sibyl.png", 0, {}, 0);

