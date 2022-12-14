#include "Object.hpp"
#include "Text.hpp"
#include "convert_string.hpp"

bool Object::execute(int id, Object& user)
{
	if (status)
	{
		if (status->first->action != nullptr)
			return status->first->action->execute(id, user, *this);
		else
			return Action::s_execute(id, user, *this);
	}
	else
	{
		if (id == Action::touch)
			return Action::event_touch(user, *this);
		else
			return false;
	}
}

void Object::damage(int v)
{
	if((status->second.hp -= v) <= 0)
		status->second.flag |= 4;
	if (status->first->type == Status::Type::player)
		TextManager::player.set((u8R"(\b)" + ext::to_u8string(v) + u8R"(のダメージを受けた。\w9\e)").c_str());
	else if(status->first->type == Status::Type::enemy)
		TextManager::partner.set((u8R"(\b)" + status->first->name + u8R"(に)" + ext::to_u8string(v) + u8R"(のダメージ。\w9\e)").c_str());
}

bool Object::has(Status::State state)const
{
	return status->second.state.find(state) != status->second.state.cend();
}

void Object::reset(int num)
{
	status->second = StatusInst(*status->first, num);
}

