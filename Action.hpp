#pragma once
#include <functional>
#include <unordered_map>

class Object;

class Action
{
	struct Ret
	{
		bool success;
		int cool;
		constexpr operator bool()const noexcept { return success; }
	};

	using ValueSingle = std::function<bool(Object&)>;
	using ValueDouble = std::function<bool(Object&, Object&)>;

	static std::unordered_map<int, ValueSingle> commonActionSingle;
	static std::unordered_map<int, ValueDouble> commonActionDouble;

public:
	std::unordered_map<int, ValueDouble> action;

	enum Key :int
	{
		item_use = 0x0000,
		item_self = 0x1000,
		enemy_action = 0x2000,
		touch = -3,
		killed = -4,
	};

	Action(std::unordered_map<int, ValueDouble> action) :action(action) {};

	static bool execute(int id, Object& user);
	bool execute(int id, Object& user, Object& target);
	static bool s_execute(int id, Object& user, Object& target);
};

