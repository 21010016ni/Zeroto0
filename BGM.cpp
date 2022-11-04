#include "BGM.hpp"
#include <DxLib.h>
#include <random>
#include <algorithm>
#include <stdexcept>
#include "convert_string.hpp"

extern std::mt19937 engine;

void BGM::set(const char8_t* file)
{
	//music.emplace_back(LoadMusicMem(file.c_str()), file.substr(0, file.find_last_of('.')));
	music.emplace_back(file);
}

void BGM::play(size_t id)
{
	if(id >= music.size())
		throw std::out_of_range("out of range");
	// buf���Z�b�g
	buf.clear();
	buf.emplace_back(&music[id]);
	// �^�C�g���X�V
	title = u8"��" + music[id].substr(0, music[id].find_last_of('.')).substr(music[id].find_last_of(u8"/\\") + 1);
	// �Đ�
	PlayMusic(ext::tochar(music[id]), DX_PLAYTYPE_BACK);
	SetVolumeMusic(volume);
}

void BGM::play(const std::u8string& file)
{
	// �^�C�g���X�V
	title = u8"��" + file.substr(0, file.find_last_of('.')).substr(file.find_last_of(u8"/\\") + 1);
	// �Đ�
	PlayMusic(ext::tochar(file), DX_PLAYTYPE_BACK);
	SetVolumeMusic(volume);
}

void BGM::stop()
{
	StopMusic();
}

bool BGM::update()
{
	// �����Ȃ��Đ�����Ă��Ȃ�������
	if(CheckMusic() == 0)
	{
		// �������[�v�Đ����L�����Đ��ł���Ȃ�����Ȃ�
		if(mode & 1 && !music.empty())
		{
			// stack���󂾂������[
			if(stack.empty())
			{
				// shuffle���L���Ȃ�shuffle
				if(mode & 2)
				{
					// buf�����Z�b�g����
					buf.clear();
					for(const auto& i : music)
						buf.emplace_back(&i);
					std::shuffle(buf.begin(), buf.end(), engine);
				}
				if(!buf.empty())
				{
					// stack���[
					for(const auto& i : buf)
						stack.emplace(i);
				}
			}
			// �^�C�g���X�V
			title = u8"��" + stack.top()->substr(0, stack.top()->find_last_of('.')).substr(stack.top()->find_last_of(u8"/\\") + 1);
			// �Đ�
			PlayMusic(ext::tochar(*stack.top()), DX_PLAYTYPE_BACK);
			SetVolumeMusic(volume);
			// �X�^�b�N�폜
			stack.pop();
			// �Ȃ̍X�V��m�点�ďI��
			return true;
		}
		// ���[�v�������A�܂��͍Đ��ł���Ȃ������Ȃ�
		else
		{
			// �^�C�g���X�V
			title.clear();
			// �Ȃ��X�V���Ă��Ȃ����Ƃ�m�点�ďI��
			return false;
		}
	}
	// �Ȃ��X�V���Ă��Ȃ����Ƃ�m�点�ďI��
	return false;
}

void BGM::ChangeVolume(unsigned char v)
{
	SetVolumeMusic(v);
	volume = v;
}

