#pragma once
#include <vector>
#include "Object.hpp"

class Manager
{
	struct Volume
	{
		float master;
		unsigned char bgm;
		unsigned char se;
		unsigned char mute;
		Volume() :master(1.0f), bgm(255), se(255), mute(0xff) {}
		bool Master() { return mute & 0x01; }
		bool Bgm() { return mute & 0x01 && mute & 0x02; }
		bool Se() { return mute & 0x01 && mute & 0x04; }
	};

	static inline int font;

public:
	static inline std::shared_ptr<Object> player;

	static inline Volume volume;

	static void preset();
	static void update();
	static void draw();
};

