#include "Action.hpp"
#include "Field.hpp"

#include "Display.hpp"
#include "Text.hpp"
#include <iostream>

bool Action::execute(int id, Object& user, Object& target)
{
	auto i = action.find(id);
	if (i == action.end())
		if ((i = commonActionDouble.find(id)) == commonActionDouble.end())
			return execute(id, user);
	return i->second(user, target);
}

bool Action::execute(int id, Object& user)
{
	auto i = commonActionSingle.find(id);
	if (i == commonActionSingle.end())
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
* -0001			�ݒ肵�Ȃ�
* -0002			�ڐG��
* -0003			�|���ꂽ�Ƃ�
* -0004
*/

enum Key :int
{
	item_use = 0x0000,
	item_self = 0x1000,
	enemy_action = 0x2000,
	touch = -1,
	killed = -2,
};

std::unordered_map<int, Action::ValueSingle> Action::commonActionSingle =
{
	{item_use + 0,[](Object&) {
		TextManager::player.set(u8R"(\b�A�N�V����0��P�ƂŎ��s\w9\e)");
		return true;
	}},
	{item_use + 1,[](Object&) {
		TextManager::player.set(u8R"(\b�A�N�V����1��P�ƂŎ��s\w9\e)");
		return true;
	}},
	{enemy_action + 0,[](Object& u) {
		// �O�i
		auto i = Field::getIterator(u.pos, -u.status->second.speedFront, true);
		if(i == Field::cend())
		{
			u.pos -= u.status->second.speedFront;
		}
		else
		{
			if(!(**i) || (*i)->status->second.flag & 1)
			{
				u.pos -= u.status->second.speedFront;
				auto j = Field::getIterator(u.pos);
				std::iter_swap(i, j);
			}
			else
			{
				u.pos = (*i)->pos + 1;
			}
		}
		u.status->second.cool = 120;	// ��
		return true;
	}},
};

std::unordered_map<int, Action::ValueDouble> Action::commonActionDouble =
{
	{item_use + 0,[](Object&,Object&) {
		TextManager::partner.set(u8R"(\b�A�N�V����0��Ώۂ�����Ď��s\w9\e)");
		return true;
	}},
	{item_use + 1,[](Object&,Object&) {
		TextManager::partner.set(u8R"(\b�A�N�V����1��Ώۂ�����Ď��s\w9\e)");
		return true;
	}},
	{enemy_action + 0,[](Object& u,Object& t) {
		// �U��
		t.status->second.hp -= u.status->second.atk;
		Display::shake.set(1, 8, 6.0f);
		u.status->second.cool = 120;	// ��
		return true;
	}},
};

