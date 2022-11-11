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
			TextManager::player.set(u8R"(\b�����g���Ĕ����J�����B\w9\e)");
			u.status->second.cool = 60;
			return true;
		}},
		{Action::Key::item_use + 0,[](Object& u, Object& t) {
			TextManager::player.set((u8R"(\b�����B�����������Ă��ĊJ���Ȃ��B\n����)" + ext::to_u8string(t.status->second.hp / u.status->second.atk) + u8R"(�������ΊJ���������B\w9\e)").c_str());
			u.status->second.cool = 20;
			return true;
		}},
	}},
	{{
		{Action::Key::item_use + 0,[](Object& u, Object& t) {
			TextManager::player.set((u8R"(\b�����B\w4)" + t.status->first->name + u8R"(�B\w9\e)").c_str());
			u.status->second.cool = 20;
			return true;
		}},
	}},
	{{
		{Action::Key::touch,[](Object& user, Object& target) {
			target.status->second.flag |= 4;
			static int num = 0;
			switch (num++)
			{
			case 0:
				TextManager::player.set(u8R"(\b�Â��A�Â��ꏊ���B\w6\n���ꂽ�ǂ�������Ɍ������肪�������ށA\n�ǂ����p�ꂽ�a�@�炵�������B\w9\e)");
				break;
			case 1:
				TextManager::player.set(u8R"(\b�i�܂Ȃ��Ă͂����Ȃ��B��ցB��ցB\n�}���ɍs���Ȃ��Ă͂����Ȃ��B\w6\n\n�c\w2�c\w2�N���H\w6\n�����񂾋^���\w2�c\w2�c\w2�����ɋQ����悤�Ȏg�����ɓh��Ԃ���Ă������B\w9\e)");
				break;
			case 2:
				TextManager::player.set(u8R"(\b���������ǂ���B�����������Ă���悤���B\w4\n���Ɍ�����ɓ��ꂽ�Ȃ�΁A���ꂪ�g���邩������Ȃ��B\w4\n�����łȂ��Ƃ��A���x���������΂��̂����J�����낤�B\w9\e)");
				break;
			case 3:
				TextManager::player.set(u8R"(\b�Ԃ̌`�������������A�K���ɉ�����ĎE���B\n��֐i�ނ��Ƃɋ��x�ɂȂ��Ă��������́A\n�܂�ł��̐�֐i�܂��܂��Ƃ��Ă��邩�̂悤�������B\w6\n\n�ז��������̂��A���������������B\w4\n�Ȃ�����Ȃɂ��Ղ��̂��낤�B\w4\n�Ȃ�����Ȃɂ��󋕂������Ă���̂��낤�B\w9\e)");
				break;
			}
			return true;
		}},
	}},
};

std::map<int, Enemy> DataBase::enemy =
{
	// ���O,HP,�U����,�O���ړ����x,����ړ����x,�˒�,�摜,�t���O,�����A�C�e��,�A�N�V�����ԍ�,���A�N�V�������̂ւ̃|�C���^
	{0,Enemy(u8"���ق����", 10, 1, 2, 0, 4, u8"data/picture/flower3868.png", 0,{{100,1},{201,1},{201,1},{202,1},{202,1},{203,1},{203,1}},0x2000,&DataBase::action[1])},
	{1,Enemy(u8"�Ǒz�����", 30, 4, 3, 0, 4, u8"data/picture/flower0497.png", 0,{{201,1},{202,1},{203,1},{102,1},{400,1},{401,1},{404,1}},0x2000,&DataBase::action[1])},
	{2,Enemy(u8"�ے肷���", 60, 7, 3, 0, 4, u8"data/picture/flower1719.png", 0,{{100,1},{103,1},{103,1},{104,1},{204,1},{206,1},{206,1},{400,1},{401,1}},0x2000,&DataBase::action[1])},
	{3,Enemy(u8"���������", 100, 10, 4, 0, 4, u8"data/picture/flower3914.png", 0,{{101,-1},{106,1},{204,1},{204,1},{205,1},{205,1},{402,1},{403,1}},0x2000,&DataBase::action[1])},
	{4,Enemy(u8"���΂����", 150, 14, 2, 0, 4, u8"data/picture/flower1720.png", 0,{{105,1},{204,2},{204,1},{207,1},{401,1},{402,1},{404,1}},0x2000,&DataBase::action[1]) },
	{100,Enemy(u8"��", 200, 0, 0, 0, 0, u8"data/picture/door.png", 0,{},-1,&DataBase::action[0])},
	{200,Enemy(u8"��", 50, 0, 0, 0, 0, u8"data/picture/door.png", 0,{},-1,&DataBase::action[0])},
	{201,Enemy(u8"�C�x���g", 0, 0, 0, 0, 0, u8"data/picture/door.png", 1,{},-1,&DataBase::action[2])},
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

