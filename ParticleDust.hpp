#pragma once
#include <DxLib.h>
#include "Particle.hpp"
#include "Point.hpp"
#include "Display.hpp"

class Dust :public Particle
{
	Point<float> pos;
	Point<float> vec;
	unsigned char alpha;

	void update()override
	{
		pos += vec;
		vec.rotate<float>(std::uniform_real_distribution{-0.2f,0.2f}(mt));
		alpha -= RangeI{0,1}(mt);
		alpha = __max(alpha, 16);
	}
	void draw()const override
	{
		//if(duration > 60)
		//	DrawCircle((int)pos.x, (int)pos.y, 1, color);
		//DrawCircle((int)pos.x, (int)pos.y, 2, color);
		SetDrawBlendMode(DX_BLENDMODE_ADD, alpha);
		DrawCircle((int)pos.x + Display::shake.x(0), (int)pos.y + Display::shake.y(0), alpha / 64, 0xffffffff);
	}

public:
	Dust() :Particle(RangeI{200,800}(mt)), pos(RangeF{0,600}(mt), RangeF{0,1024}(mt)), vec(RangeF{-0.1f,0.1f}(mt), RangeF{-0.3f,0.3f}(mt)), alpha(RangeI{64,192}(mt))
	{
	}
};

