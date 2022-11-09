#include "Effect.hpp"
#include <DxLib.h>
#include "HandleManager.hpp"

Effect::Data::Data(std::u8string graph, int wn, int hn, int wait, std::u8string se, unsigned char volume) :graph(graph), num(hn, wn), wait(wait), se(se), volume(volume)
{
	GetGraphSize(Handle::get(graph, Handle::Type::graph), &size.x, &size.y);
	size /= num;
}

void Effect::Data::draw(int x, int y, int t)const
{
	t /= wait;
	DrawRectGraph(x, y, (t % num.x) * size.x, (t / num.x) * size.y, size.x, size.y, Handle::get(graph, Handle::Type::graph), TRUE);
}

void Effect::Data::play()const
{
	PlaySoundMem(Handle::get(se, Handle::Type::sound), DX_PLAYTYPE_BACK);
}

Effect::Inst::Inst(const Data* data, int x, int y, Pos pos) :data(data), dst(y, x), time(0), pos(pos)
{

}

void Effect::Inst::play()const
{
	if(time == 0)
		data->play();
	switch(pos)
	{
	case Pos::leftup:
		data->draw(dst.x, dst.y, time);
		break;
	case Pos::rightup:
		data->draw(dst.x - data->size.x, dst.y, time);
		break;
	case Pos::leftdown:
		data->draw(dst.x, dst.y - data->size.y, time);
		break;
	case Pos::rightdown:
		data->draw(dst.x - data->size.x, dst.y - data->size.y, time);
		break;
	case Pos::center:
		data->draw(dst.x - data->size.x / 2, dst.y - data->size.y / 2, time);
		break;
	}
}

void Effect::load(std::u8string graph, int wn, int hn, int wait, std::u8string se)
{
	data.emplace_back(graph, wn, hn, wait, se, volume);
}

void Effect::load(std::u8string se)
{
	data.emplace_back(u8"", 1, 1, 1, se, volume);
}

void Effect::set(int id, int x, int y, Pos pos)
{
	list.emplace_back(&data.at(id), x, y, pos);
}

void Effect::play()
{
	for(auto i = list.begin(); i != list.end();)
	{
		if(i->data->volume != volume)
		{
			ChangeVolumeSoundMem(volume, Handle::get(i->data->se, Handle::Type::sound));
			i->data->volume = volume;
		}
		i->play();
		if (++(i->time) >= i->data->size.y * i->data->size.x * i->data->wait)
		{
			i = list.erase(i);
			continue;
		}
		++i;
	}
}

