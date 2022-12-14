#include "Field.hpp"

std::weak_ptr<Field::Value> Field::get(int pos, int range, bool force,bool checkEvent)
{
	auto it = list.begin();
	if(range > 0)
	{
		for(; it != list.end(); ++it)
			if((*it)->pos > pos && ((**it) || force) && ((*it)->type() != Status::Type::ev || checkEvent))
				break;
		if(it != list.end() && (*it)->pos <= pos + range)
			return *it;
		return std::weak_ptr<Value>();
	}
	else
	{
		auto it = list.end();
		while (it != list.begin())
			if ((*--it)->pos < pos)
			{
				if (((**it) || force) && ((*it)->type() != Status::Type::ev || checkEvent))
					if ((*it)->pos >= pos + range)
						return *it;
			}
		return std::weak_ptr<Value>();
	}
}

std::list<std::shared_ptr<Field::Value>>::iterator Field::getIterator(int pos, int range, bool force, bool checkEvent)
{
	if(range > 0)
	{
		auto it = list.begin();
		for(; it != list.end(); ++it)
			if((*it)->pos > pos && ((**it) || force) && ((*it)->type() != Status::Type::ev || checkEvent))
				break;
		if (it != list.end() && (*it)->pos <= pos + range)
			return it;
		return list.end();
	}
	else
	{
		auto it = list.end();
		while (it != list.begin())
			if ((*--it)->pos < pos)
			{
				if (((**it) || force) && ((*it)->type() != Status::Type::ev || checkEvent))
					if ((*it)->pos >= pos + range)
						return it;
			}
		return list.end();
	}
}
std::list<std::shared_ptr<Field::Value>>::iterator Field::getIterator(int pos)
{
	auto it = list.begin();
	for(; it != list.end(); ++it)
		if((*it)->pos == pos)
			return it;
	return list.end();
}

//std::list<std::shared_ptr<Field::Value>>::iterator Field::getIterator(const std::shared_ptr<Value>& t)
//{
//	for(auto it = list.begin(); it != list.end(); ++it)
//		if(*it == t)
//			return it;
//	return list.end();
//}

std::shared_ptr<Field::Value> Field::set(Value* t)
{
	for(auto i = list.cbegin(); i != list.cend(); ++i)
	{
		if((*i)->pos > t->pos)
		{
			return *(list.emplace(i, t));
		}
	}
	list.emplace_back(t);
	return list.back();
}

//void Field::set(std::shared_ptr<Value>& t)
//{
//	for(auto i = list.cbegin(); i != list.cend(); ++i)
//	{
//		if((*i)->pos > t->pos)
//		{
//			list.emplace(i, t);
//			return;
//		}
//	}
//	list.emplace_back(t);
//}

