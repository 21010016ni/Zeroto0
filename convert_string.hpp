#pragma once
#include <string>

// ���ʐ݌v�Ƃ��āA�ʂ�utf-8�����R�[�h��Shift-JIS�����R�[�h�ɕϊ�����E�܂����̋t������Ƃ����킯�ł͂Ȃ�
// �����܂ŕێ�����f�[�^���̂��͓̂����ŁA���X�̊֐��Ŏ󂯎���ď�������ۂ�u8string���g���Ȃ����Ƃւ̑΍�
// utf-8�Ȃ�A���p�����Ɋւ��Ă�ASCII�Ɠ��������R�[�h�̂��߁A�ϊ����Ă�Shift-JIS�I�ɉ��߂ł��Ȃ��������܂܂��悤�ɂȂ邾���ő������Ȃ��͂�
// �����Aformat�őS�p�����ɒu�����悤�Ƃ�����ƃ_�������@���̂�����͋C��t����悤��
// �}�W�Łc�c���X�̊֐���u8string�ɑΉ������炻��ŉ�������b�Ȃ�ł����H�H�H�H�H�@C++�H�H�H�H�H�H�@����

	// string��u8string��������
std::string operator<<=(std::string& s, const std::u8string& u8);

// u8string��string��������
std::u8string operator<<=(std::u8string& u8, const std::string& s);

namespace ext
{

	// u8string��string�ɕϊ��������̂�Ԃ�
	std::string convert(const std::u8string& u8);

	// string��u8string�ɕϊ��������̂�Ԃ�
	std::u8string convert(const std::string& s);

	// u8string�̕�����const char*�Ƃ��ĕԂ��i�W����c_str�֐���const char8_t*�j
	const char* tochar(const std::u8string& u8);
}

