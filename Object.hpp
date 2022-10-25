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
	void damage(int v);

	operator bool()const noexcept { return static_cast<bool>(status); }
};

class Player :public Object
{
public:
	int partner;
	int searchRange;
	std::vector<int> shortcut;

	Player(int pos, Status* status, int partner, int searchRange) :Object(pos, status), partner(partner), searchRange(searchRange)
	{
		shortcut.resize(64, -1);
		type = Type::player;
	}
};

