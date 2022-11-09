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
		bool BGM() { return mute & 0x01 && mute & 0x02; }
		bool SE() { return mute & 0x01 && mute & 0x04; }
		unsigned char GetBGM() { return BGM() ? static_cast<unsigned char>(master * bgm) : 0; }
		unsigned char GetSE() { return SE() ? static_cast<unsigned char>(master * se) : 0; }
	};

	static inline int font;
	static inline int back;
	static Display display;
	static Display ui;
	static inline bool displayControll = true;

	static std::vector<int> cont;

	static inline int var[3] = {0,0,0};

public:
	enum class GameState
	{
		title,
		config,
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

