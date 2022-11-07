#pragma once
#include <vector>
#include "Object.hpp"
#include "Display.hpp"

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
	static inline int back;
	static Display display;
	static Display ui;

	static std::vector<int> cont;

	static inline int var[3] = {0,0,0};

public:
	enum class GameState
	{
		title,
		play,
		over,
	};

	static inline GameState gameState;

	static inline std::shared_ptr<Object> player;

	static inline Volume volume;

	static bool save();
	static bool load();

	static void preset();
	static bool update();
	static void draw();
};

