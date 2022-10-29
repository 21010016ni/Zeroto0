#include "Action.hpp"
#include "Field.hpp"

#include "Display.hpp"
#include "Text.hpp"
#include "convert_string.hpp"
#include <iostream>

bool Action::execute(int id, Object& user, Object& target)
{
	auto i = action.find(id);
	if (i == action.end())
		if ((i = commonActionDouble.find(id)) == commonActionDouble.end())
			return execute(id, user);
	return i->second(user, target);
}

bool Action::s_execute(int id, Object& user, Object& target)
{
	auto i = commonActionDouble.find(id);
	if (i == commonActionDouble.end())
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
* -0002			�A�C�e�����ݒ�
* -0003			�ڐG��
* -0004			�|���ꂽ�Ƃ�
*/

std::unordered_map<int, Action::ValueSingle> Action::commonActionSingle =
{
	{touch,[](Object& u) {	// �ڐG��
		TextManager::player.set(u8R"(\bDebug:�I�u�W�F�N�g�ɐڐG����\w9\e)");
		return true;
	}},
	{-2,[](Object& u) {	// �A�C�e�����ݒ�
		TextManager::player.set(u8R"(\b�c�c�g������������Ȃ��B\w9\e)");
		return false;
	}},
	{item_use + 0,[](Object& u) {	// ���ׂ�
		TextManager::player.set(u8R"(\b�����Ȃ��B\w9\e)");
		u.status->second.cool = 20;
		return false;
	}},
	{item_use + 1,[](Object& u) {	// �b��
		TextManager::player.set(u8R"(\b����Ɍ������Ęb���������B\w9\e)");
		u.status->second.cool = 20;
		return false;
	}},
	{item_use + 2,[](Object& u) {	// ����
		TextManager::player.set(u8R"(\b�����Ȃ��Ƃ������������B\w9\e)");
		u.status->second.cool = 20;
		return false;
	}},
	{item_use + 3,[](Object& u) {	// ���Ƙb��
		switch(static_cast<Player*>(u.status->first)->partner)
		{
			case 4:
				TextManager::player.set(u8R"(\b�u\w6�c\w6�c\w6��A\w2�ǂ������́A\w2���Z�����H�v\w9\e)");
				break;
			default:
				TextManager::player.set(u8R"(\b�����ɖ��͂��Ȃ��B\w9\e)");
				break;
		}
		u.status->second.cool = 60;
		return true;
	}},
	{item_use + 100,[](Object& u) {	// ��
		TextManager::player.set(u8R"(\b�����g�������Ȃ��B\w9\e)");
		u.status->second.cool = 20;
		return false;
	}},
	{item_use + 102,[](Object& u) {	// �񕜖�
		u.status->second.hp += 20;
		TextManager::player.set(u8R"(\b������݊����A20�񕜂����B\w9\e)");
		u.status->second.cool = 60;
		return true;
	}},
	{item_use + 103,[](Object& u) {	// �Ŗ�
		if((u.status->second.hp -= 80) <= 0)
			u.status->second.flag |= 4;
		TextManager::player.set(u8R"(\b�Ŗ�����݊����A80�̃_���[�W���󂯂��B\w9\e)");
		u.status->second.cool = 60;
		return true;
	}},
	{item_use + 104,[](Object& u) {	// ���ٓ�
		u.status->second.state[Status::State::saturation] = 20;
		TextManager::player.set(u8R"(\b�ٓ���H�ׂ��B\w6\n�c�c�����ɂȂ�A�̂Ɋ��͂�����B�������X�ɖ����Ă����B\w9\e)");
		u.status->second.cool = 60;
		return true;
	}},
	{item_use + 105,[](Object& u) {	// �X�C
		u.status->second.state[Status::State::bright] = 60;
		TextManager::player.set(u8R"(\b�X�C�ɉ΂𓔂����B\w6\n�c�c�ӂ肪�Ƃ炳��ėǂ�������B\w9\e)");
		u.status->second.cool = 60;
		return true;
	}},
	{item_use + 106,[](Object& u) {	// ������
		u.status->second.state[Status::State::arousal] = 60;
		TextManager::player.set(u8R"(\b�����܂����݊������B\n�̂��ΏƂ�A���̂Ǝv�l����Ⰲ��O�����̂�������B\w9\e)");
		u.status->second.cool = 60;
		return true;
	}},
	{item_use + 200,[](Object& u) {	// �K�т��Z��
		TextManager::player.set(u8R"(\b�K�т��Z�����y���U�����B\w9\e)");
		u.status->second.cool = 20;
		return false;
	}},
	{item_use + 201,[](Object& u) {	// �V���[�g�\�[�h
		TextManager::player.set(u8R"(\b�V���[�g�\�[�h�̓��g�͓݂��P����ттĂ���B\w9\e)");
		u.status->second.cool = 20;
		return false;
	}},
	{item_use + 202,[](Object& u) {	// �n���h�A�b�N�X
		TextManager::player.set(u8R"(\b�蕀�̏d�����n���y�����ł��B\w9\e)");
		u.status->second.cool = 20;
		return false;
	}},
	{item_use + 203,[](Object& u) {	// �V���[�g�X�s�A
		TextManager::player.set(u8R"(\b�悭����ΐ�[�����������Ă���B\w9\e)");
		u.status->second.cool = 20;
		return false;
	}},
	{item_use + 300,[](Object& u) {	// ���ԋl�߂̖�
		TextManager::player.set(u8R"(\b�ǂ����肪���ė��������B\n�ǂ������������悤�Ȋ��S������B\w9\e)");
		u.status->second.cool = 20;
		return false;
	}},
	{item_use + 301,[](Object& u) {	// ���Ԃ̓��L��
		TextManager::player.set(u8R"(\b���g�͑S�Ĕ����̂悤���B\n�ǂ�ȋL�^�ł��L���Ă����邾�낤�B\w9\e)");
		u.status->second.cool = 20;
		return false;
	}},
	{item_use + 302,[](Object& u) {	// �����Ԃ̞x
		TextManager::player.set(u8R"(\b����̏��Ȃ��V���v���Ȟx���B\n���݂��߂�{��T�������Ȃ�B\w9\e)");
		u.status->second.cool = 20;
		return false;
	}},
	{item_use + 303,[](Object& u) {	// �Ԃ���|�v��
		TextManager::player.set(u8R"(\b�r�̊W�����������J���Ă݂��B\n�S�����炮�悤�ȍ��肪�L�������B\w9\e)");
		u.status->second.cool = 20;
		return false;
	}},
	{item_use + 304,[](Object& u) {	// ����߂��Ή�
		TextManager::player.set(u8R"(\b�Ԃт�̂悤�Ȑ΂ɂ����ƐG�ꂽ�B\n�󂵂Ă��܂������ł����Ɏ�𗣂����B\w9\e)");
		u.status->second.cool = 20;
		return false;
	}},
	{item_use + 305,[](Object& u) {	// �ԏ���̍��C
		TextManager::player.set(u8R"(\b�Ԃ̏��肪�����ɗh�ꂽ�B\n�����������A�������Ƃ͂ł��Ȃ����낤�B\w9\e)");
		u.status->second.cool = 20;
		return false;
	}},
	{enemy_action + 0,[](Object& u) {	// �O�i
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
	{item_use + 1,[](Object& u,Object&) {	// �b��
		TextManager::partner.set(u8R"(\b����͌��t��Ԃ��Ȃ��B\w9\e)");
		u.status->second.cool = 20;
		return false;
	}},
	{item_use + 2,[](Object& u,Object& t) {	// ����
		t.damage(static_cast<int>(u.status->second.atk* (u.has(Status::State::arousal) ? 1.3f : 1.0f)));
		u.status->second.cool = 60;
		return true;
	}},
	{item_use + 103,[](Object& u,Object& t) {	// �Ŗ�
		if((t.status->second.hp -= 20) <= 0)
			t.status->second.flag |= 4;
		TextManager::partner.set(u8R"(\b�Ŗ�𓊂����A20�̃_���[�W��^�����B\w9\e)");
		u.status->second.cool = 60;
		return true;
	}},
	{item_use + 200,[](Object& u,Object& t) {	// �K�т��Z��
		t.damage(static_cast<int>(u.status->second.atk* (u.has(Status::State::arousal) ? 1.3f : 1.0f)));
		u.status->second.cool = 40;
		return true;
	}},
	{item_use + 201,[](Object& u,Object& t) {	// �V���[�g�\�[�h
		t.damage(static_cast<int>((u.status->second.atk + 2)* (u.has(Status::State::arousal) ? 1.3f : 1.0f)));
		u.status->second.cool = 50;
		return true;
	}},
	{item_use + 202,[](Object& u,Object& t) {	// �n���h�A�b�N�X
		t.damage(static_cast<int>((u.status->second.atk + 8)* (u.has(Status::State::arousal) ? 1.3f : 1.0f)));
		u.status->second.cool = 90;
		return true;
	}},
	{item_use + 203,[](Object& u,Object& t) {	// �V���[�g�X�s�A
		t.damage(static_cast<int>((u.status->second.atk + 3)* (u.has(Status::State::arousal) ? 1.3f : 1.0f)));
		u.status->second.cool = 70;
		return true;
	}},
	{enemy_action + 0,[](Object& u,Object& t) {	// �U��
		t.damage(static_cast<int>(u.status->second.atk* (u.has(Status::State::arousal) ? 1.3f : 1.0f)));
		Display::shake.set(1, 8, 6.0f);
		u.status->second.cool = 120;	// ��
		return true;
	}},
};

