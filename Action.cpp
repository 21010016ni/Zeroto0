#include "Action.hpp"
#include "Field.hpp"

#include <iostream>

bool Action::execute(int id, Object& user, Object& target)
{
	auto i = action.find(id);
	if(i == action.end())
		if((i = commonActionDouble.find(id)) == commonActionDouble.end())
			return execute(id, user);
	return i->second(user, target);
}

bool Action::execute(int id, Object& user)
{
	auto i = commonActionSingle.find(id);
	if(i == commonActionSingle.end())
	{
		// �G���[����
		return false;
	}
	return i->second(user);
}

/*
* �����A�A�N�V����ID�̓A�C�e��ID�Ɠ����l��ݒ肷��
* �r�b�g���Z�̉\�����c�����߂ɁA16�i���Őݒ�
* +0000 ~ +0fff	�A�C�e���𑊎�Ɏg�p
* +1000 ~ +1fff	�A�C�e�������g�Ɏg�p
* -0001			�ڐG��
* -0002			�|���ꂽ�Ƃ�
* -0003			
*/

enum Key :int
{
	item_use = 0x0000,
	item_self = 0x1000,
	touch = -1,
	killed = -2,
};

std::unordered_map<int, Action::ValueSingle> Action::commonActionSingle =
{
	{item_use + 0,[](Object&) { std::cout << "�A�N�V����0��P�ƂŎ��s" << std::endl; return true; }},
	{item_use + 1,[](Object&) { std::cout << "�A�N�V����1��P�ƂŎ��s" << std::endl; return true; }},
};

std::unordered_map<int, Action::ValueDouble> Action::commonActionDouble =
{
	{item_use + 0,[](Object&,Object&) { std::cout << "�A�N�V����0��Ώۂ�����Ď��s" << std::endl; return true; }},
	{item_use + 1,[](Object&,Object&) { std::cout << "�A�N�V����1��Ώۂ�����Ď��s" << std::endl; return true; }},
};

