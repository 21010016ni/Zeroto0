#pragma once
#include <memory>
#include "Action.hpp"
#include "Status.hpp"

class Object
{
	Action action;

public:
	std::unique_ptr<std::pair<Status*, StatusInst>> status;
	int pos;

	Object(int pos) :pos(pos) {}
	Object(int pos, Status* status) :pos(pos), status(new std::pair<Status*, StatusInst>(status, StatusInst(*status))) {}

	bool execute(int id, Object& user);
	void damage(int v);

	operator bool()const noexcept { return static_cast<bool>(status); }
};

