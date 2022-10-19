#include "Field.hpp"

std::weak_ptr<Field::Value> Field::get(int pos, int range, bool force)
{
	auto it = list.begin();
	if(range > 0)
	{
		for(; it != list.end(); ++it)
			if((*it)->pos > pos)
				break;
		if(it != list.end() && (*it)->pos <= pos + range && ((**it) || force))
			return *it;
		return std::weak_ptr<Value>();
	}
	else
	{
		for(; it != list.end(); ++it)
		{
			if((*it)->pos >= pos)
				break;
			else if((*it)->pos >= pos + range && (**it))
				return *it;
		}
		return std::weak_ptr<Value>();
	}
}

std::list<std::shared_ptr<Field::Value>>::iterator Field::getIterator(int pos, int range, bool force)
{
	auto it = list.begin();
	if(range > 0)
	{
		for(; it != list.end(); ++it)
			if((*it)->pos > pos)
				break;
		if(it != list.end() && (*it)->pos <= pos + range && ((**it) || force))
			return it;
		return list.end();
	}
	else
	{
		for(; it != list.end(); ++it)
			if((*it)->pos >= pos)
				break;
			else if((*it)->pos >= pos + range && (**it))
				return it;
		return list.end();
	}
}

std::list<std::shared_ptr<Field::Value>>::iterator Field::getIterator(const std::shared_ptr<Value>& t)
{
	for(auto it = list.begin(); it != list.end(); ++it)
		if(*it == t)
			return it;
	return list.end();
}

void Field::set(Value* t)
{
	for(auto i = list.cbegin(); i != list.cend(); ++i)
	{
		if((*i)->pos > t->pos)
		{
			list.emplace(i, t);
			return;
		}
	}
	list.emplace_back(t);
}

void Field::set(std::shared_ptr<Value>& t)
{
	for(auto i = list.cbegin(); i != list.cend(); ++i)
	{
		if((*i)->pos > t->pos)
		{
			list.emplace(i, t);
			return;
		}
	}
	list.emplace_back(t);
}

