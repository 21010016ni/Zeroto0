#include "DataBase.hpp"
#include <fstream>
#include <boost/algorithm/string.hpp>
#include "convert_string.hpp"

std::vector<Item> DataBase::enemyAction =
{
	{u8"Enemy0", -1,-0x1000,4},
};

std::vector<Status> DataBase::status =
{
	{10, 10, u8"data/picture/flower0497.png", 0},
	{10, 10, u8"data/picture/flower3868.png", 1},
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
		if (elem.size() != 4)
			continue;
		item.emplace_back(ext::convert(elem[0]).c_str(), std::stoi(elem[1], nullptr, 16), std::stoi(elem[2]), std::stoi(elem[3]));
	}
}

