#pragma once
#include <memory>
#include "Status.hpp"

class Object
{
public:
	std::unique_ptr<std::pair<Status*, StatusInst>> status;
	int pos;

	Object(int pos) :pos(pos) {}
	Object(int pos, Status* status, int num) :pos(pos), status(new std::pair<Status*, StatusInst>(status, StatusInst(*status, num))) {}

	bool execute(int id, Object& user);
	void damage(int v);
	bool has(Status::State state)const;
	void reset(int num);
	Status::Type type() { return status ? status->first->type : Status::Type::ev; }

	operator bool()const noexcept { return static_cast<bool>(status); }
};

