#include "Action.hpp"
#include "Field.hpp"

bool Action::execute(int id, Object& user, Object& target)
{
	auto i = action.find(id);
	if(i == action.end())
		if((i = commonActionDouble.find(id)) == commonActionDouble.end())
			return execute(id, user);
	return i->second(user, target);
}

bool Action::execute(int id, Object& user)
{
	auto i = commonActionSingle.find(id);
	if(i == commonActionSingle.end())
	{
		// ƒGƒ‰[ˆ—
		return false;
	}
	return i->second(user);
}

std::unordered_map<int, Action::ValueSingle> Action::commonActionSingle =
{
	{0,[](Object&) { return true; }},
};

std::unordered_map<int, Action::ValueDouble> Action::commonActionDouble =
{
	{0,[](Object&,Object&) { return true; }},
};

