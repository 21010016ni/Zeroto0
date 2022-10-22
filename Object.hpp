#pragma once
#include <memory>
#include "Action.hpp"
#include "Status.hpp"

class Object
{
	Action action;

public:
	enum class Type
	{
		enemy,
		player,
	};
	Type type;

	std::unique_ptr<std::pair<Status*, StatusInst>> status;
	int pos;

	Object(int pos) :type(Type::enemy), pos(pos) {}
	Object(int pos, Status* status) :type(Type::enemy), pos(pos), status(new std::pair<Status*, StatusInst>(status, StatusInst(*status))) {}

	bool execute(int id, Object& user);

	operator bool()const noexcept { return static_cast<bool>(status); }
};

class Player :public Object
{
public:
	int searchRange;
	std::vector<int> shortcut;

	Player(int pos, Status* status, int searchRange) :Object(pos, status), searchRange(searchRange)
	{
		shortcut.resize(64, -1);
		type = Type::player;
	}
};

