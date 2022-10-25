#pragma once
#include <string>
#include <map>

class Status
{
public:
	enum class State :char
	{
		poison,
		saturation,
		bright,
		arousal,
	};

	std::u8string name;
	int hp;
	int atk;
	int speedFront;
	int speedBack;
	std::u8string graph;

	unsigned char flag;	// �ŉ��ʃr�b�g�͒ʍs�s�̔���ɗp����@����ȊO�͎��R

	Status(const char8_t* name, int hp, int atk, int speedFront, int speedBack, const char8_t* graph, unsigned char flag) :name(name), hp(hp), atk(atk), speedFront(speedFront), speedBack(speedBack), graph(graph), flag(flag) {}
	
	std::map<int, int> item;	// �G�̏ꍇ�h���b�v�A�C�e���A�v���C���[�̏ꍇ���p���A�C�e��
};

class StatusInst
{
public:
	int hp;
	int atk;
	int cool;
	int speedFront;
	int speedBack;

	unsigned char flag;	// -,-,-,-, -,���^�C�~���O�ō폜,�ڐG�ς�,�ʍs�s��

	std::map<int, int> item;	// �����A�C�e���@�G�͂����Status����R�s�[���Ď󂯎��A�h���b�v����
	std::map<Status::State, int> state;	// ���ݏ�ԁAsecond�͎���

	StatusInst(const Status& t);

	void AddItem(const std::map<int, int>& t);
};