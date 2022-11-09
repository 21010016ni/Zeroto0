#pragma once
#include <string>
#include <vector>
#include <list>
#include "Point.hpp"

class Effect
{
public:
	enum class Pos :char
	{
		leftup,
		rightup,
		leftdown,
		rightdown,
		center
	};

private:
	struct Data
	{
		std::u8string graph;
		Point<int> size;
		Point<int> num;
		int wait;
		std::u8string se;
		mutable unsigned char volume;

		Data(std::u8string graph, int wn, int hn, int wait, std::u8string se, unsigned char volume);

		void draw(int x, int y, int t)const;
		void play()const;
	};
	struct Inst
	{
		const Data* data;
		Point<int> dst;
		int time;
		Pos pos;

		Inst(const Data* data, int x, int y, Pos pos);

		void play()const;
	};

	static inline std::vector<Data> data;
	static inline std::list<Inst> list;

public:
	static inline unsigned char volume = 255;

	static void load(std::u8string graph, int wn, int hn, int wait, std::u8string se);
	static void load(std::u8string se);

	static void set(int id, int x, int y, Pos pos);
	static void play();
};

