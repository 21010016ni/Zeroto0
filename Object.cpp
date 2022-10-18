#include "Object.hpp"

bool Object::execute(int id, Object & user)
{
	return action.execute(id, user);
}
