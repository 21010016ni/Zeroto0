#include "DataBase.hpp"
#include <fstream>
#include <boost/algorithm/string.hpp>
#include "convert_string.hpp"

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
