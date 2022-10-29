#pragma once
#include <memory>
#include "Status.hpp"

class Object
{
public:
	std::unique_ptr<std::pair<Status*, StatusInst>> status;
	int pos;

	Object(int pos) :pos(pos) {}
	Object(int pos, Status* status) :pos(pos), status(new std::pair<Status*, StatusInst>(status, StatusInst(*status))) {}

	bool execute(int id, Object& user);
	void damage(int v);
	bool has(Status::State state)const;

	operator bool()const noexcept { return static_cast<bool>(status); }
};

