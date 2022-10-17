#pragma once
#include <map>

class Status
{
public:
	int hp;
	int atk;

	unsigned char flag;	// �ŉ��ʃr�b�g�͒ʍs�s�̔���ɗp����@����ȊO�͎��R

	Status(int hp, int atk, unsigned char flag) :hp(hp), atk(atk), flag(flag) {}
	
	std::map<int, int> item;	// �G�̏ꍇ�h���b�v�A�C�e���A�v���C���[�̏ꍇ���p���A�C�e��
};

class StatusInst
{
public:
	int hp;
	int atk;
	int cool;

	unsigned char flag;

	std::map<int, int> item;	// �����A�C�e���@�G�͂����Status����R�s�[���Ď󂯎��A�h���b�v����

	StatusInst(const Status& t);

	void AddItem(const std::map<int, int>& t);
};