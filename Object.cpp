#include "Object.hpp"
#include "Text.hpp"
#include "convert_string.hpp"

bool Object::execute(int id, Object& user)
{
	return action.execute(id, user, *this);
}

void Object::damage(int v)
{
	if((status->second.hp -= v) <= 0)
		status->second.flag |= 4;
	switch(type)
	{
	case Object::Type::enemy:
		TextManager::partner.set((u8R"(\b)" + status->first->name + u8R"(に)" + ext::to_u8string(v) + u8R"(のダメージ。\w9\e)").c_str());
		break;
	case Object::Type::player:
		TextManager::player.set((u8R"(\b)" + ext::to_u8string(v) + u8R"(のダメージを受けた。\w9\e)").c_str());
		break;
	}
}

