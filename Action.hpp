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

	std::unordered_map<int, ValueDouble> action;

public:
	static bool execute(int id, Object& user);
	bool execute(int id, Object& user, Object& target);
};

