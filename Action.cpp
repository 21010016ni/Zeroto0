#include "Action.hpp"
#include "Field.hpp"

#include "Display.hpp"
#include "Text.hpp"
#include "Effect.hpp"
#include "convert_string.hpp"

bool Action::execute(int id, Object& user, Object& target)
{
	auto i = action.find(id);
	if (i == action.end())
		if ((i = commonActionDouble.find(id)) == commonActionDouble.end())
			return execute(id, user);
	return i->second(user, target);
}

bool Action::s_execute(int id, Object& user, Object& target)
{
	auto i = commonActionDouble.find(id);
	if (i == commonActionDouble.end())
		return execute(id, user);
	return i->second(user, target);
}

bool Action::execute(int id, Object& user)
{
	auto i = commonActionSingle.find(id);
	if (i == commonActionSingle.end())
	{
		// エラー処理
		return false;
	}
	return i->second(user);
}

/*
* 原則、アクションIDはアイテムIDと同じ値を設定する
* ビット演算の可能性を残すために、16進数で設定
* +0000 ~ +0fff	アイテムを相手に使用
* +1000 ~ +1fff	アイテムを自身に使用
* -0001			設定しない
* -0002			アイテム未設定
* -0003			接触時
* -0004			倒されたとき
*/

std::unordered_map<int, Action::ValueSingle> Action::commonActionSingle =
{
	{touch,[](Object& u) {	// 接触時
		//TextManager::player.set(u8R"(\bDebug:オブジェクトに接触した\w9\e)");
		return true;
	}},
	{killed,[](Object& u) {	// 倒された
		TextManager::player.add(u8R"(\bふらり、体が崩れる。\w4\n視界が歪む、体が熱いような、冷たいような奇妙な感覚。\w6\n\nやがて床が目の前に猛烈な勢いで迫る。\w6\n違う、己が倒れているのだ。\w4\n\n……呆気なく、こうして倒れた。\w6\n死んだ。\w9\n\n（何れかのキーでタイトルへ戻る）\w9\w9\e)");
		return true;
	}},
	{-2,[](Object& u) {	// アイテム未設定
		TextManager::player.set(u8R"(\b……使い方が分からない。\w9\e)");
		return false;
	}},
	{item_use + 0,[](Object& u) {	// 調べる
		TextManager::player.set(u8R"(\b何もない。\w9\e)");
		u.status->second.cool = 20;
		return false;
	}},
	{item_use + 1,[](Object& u) {	// 話す
		TextManager::player.set(u8R"(\b虚空に向かって話しかけた。\w9\e)");
		u.status->second.cool = 20;
		return false;
	}},
	{item_use + 2,[](Object& u) {	// 殴る
		TextManager::player.set(u8R"(\b何もないところを殴りつけた。\w9\e)");
		u.status->second.cool = 20;
		return false;
	}},
	{item_use + 3,[](Object& u) {	// 妹と話す
		switch(static_cast<Player*>(u.status->first)->partner)
		{
			case 4:
				TextManager::player.set(u8R"(\b「\w6…\w6…\w6ん、\w2どうしたの、\w2お兄ちゃん？」\w9\e)");
				break;
			default:
				TextManager::player.set(u8R"(\bここに妹はいない。\w9\e)");
				break;
		}
		u.status->second.cool = 60;
		return true;
	}},
	{item_use + 100,[](Object& u) {	// 鍵
		TextManager::player.set(u8R"(\b鍵を使う錠がない。\w9\e)");
		u.status->second.cool = 20;
		return false;
	}},
	{item_use + 102,[](Object& u) {	// 回復薬
		u.status->second.hp += 20;
		TextManager::player.set(u8R"(\b薬を飲み干し、20回復した。\w9\e)");
		u.status->second.cool = 60;
		return true;
	}},
	{item_use + 103,[](Object& u) {	// 毒薬
		TextManager::player.set(u8R"(\b毒薬を飲み干し、80のダメージを受けた。\w9\e)");
		if ((u.status->second.hp -= 80) <= 0)
		{
			u.status->second.flag |= 4;
			Action::execute(killed, u);
		}
		u.status->second.cool = 60;
		return true;
	}},
	{item_use + 104,[](Object& u) {	// お弁当
		u.status->second.state[Status::State::saturation] = 20;
		TextManager::player.set(u8R"(\b弁当を食べた。\w6\n……満腹になり、体に活力が漲る。傷が徐々に癒えていく。\w9\e)");
		u.status->second.cool = 60;
		return true;
	}},
	{item_use + 105,[](Object& u) {	// 蝋燭
		u.status->second.state[Status::State::bright] = 60;
		TextManager::player.set(u8R"(\b蝋燭に火を灯した。\w6\n……辺りが照らされて良く見える。\w9\e)");
		u.status->second.cool = 60;
		return true;
	}},
	{item_use + 106,[](Object& u) {	// 興奮剤
		u.status->second.state[Status::State::arousal] = 60;
		TextManager::player.set(u8R"(\b興奮剤を飲み干した。\n体が火照る、肉体と思考から箍が外されるのを感じる。\w9\e)");
		u.status->second.cool = 60;
		return true;
	}},
	{item_use + 200,[](Object& u) {	// 錆びた短剣
		TextManager::player.set(u8R"(\b錆びた短剣を軽く振った。\w9\e)");
		u.status->second.cool = 20;
		return false;
	}},
	{item_use + 201,[](Object& u) {	// ショートソード
		TextManager::player.set(u8R"(\bショートソードの刀身は鈍い輝きを帯びている。\w9\e)");
		u.status->second.cool = 20;
		return false;
	}},
	{item_use + 202,[](Object& u) {	// ハンドアックス
		TextManager::player.set(u8R"(\b手斧の重たい刃を軽く撫でた。\w9\e)");
		u.status->second.cool = 20;
		return false;
	}},
	{item_use + 203,[](Object& u) {	// ショートスピア
		TextManager::player.set(u8R"(\bよく見れば先端が少し欠けている。\w9\e)");
		u.status->second.cool = 20;
		return false;
	}},
	{item_use + 204,[](Object& u) {	// レイピア
		TextManager::player.set(u8R"(\b軽く振ってしなりを確かめた。\w9\e)");
		u.status->second.cool = 20;
		return false;
	}},
	{item_use + 205,[](Object& u) {	// バトルアックス
		TextManager::player.set(u8R"(\b素振りをした。……体が持っていかれそうになった。\w9\e)");
		u.status->second.cool = 20;
		return false;
	}},
	{item_use + 206,[](Object& u) {	// ハルバード
		TextManager::player.set(u8R"(\b演舞の真似事をした。\n……見るに堪えない、ただの槍としてしか使えないだろう。\w9\e)");
		u.status->second.cool = 20;
		return false;
	}},
	{item_use + 207,[](Object& u) {	// グラディウス
		TextManager::player.set(u8R"(\b刀身が光を返してぎらりと煌めいた。\w9\e)");
		u.status->second.cool = 20;
		return false;
	}},
	{item_use + 300,[](Object& u) {	// 香花詰めの枕
		TextManager::player.set(u8R"(\b良い香りがして落ち着く。\nどこか懐かしいような感慨を抱く。\w9\e)");
		u.status->second.cool = 20;
		return false;
	}},
	{item_use + 301,[](Object& u) {	// 白花の日記帳
		TextManager::player.set(u8R"(\b中身は全て白紙のようだ。\nどんな記録でも記していけるだろう。\w9\e)");
		u.status->second.cool = 20;
		return false;
	}},
	{item_use + 302,[](Object& u) {	// 押し花の栞
		TextManager::player.set(u8R"(\b飾りの少ないシンプルな栞だ。\n挟みこめる本を探したくなる。\w9\e)");
		u.status->second.cool = 20;
		return false;
	}},
	{item_use + 303,[](Object& u) {	// 花がらポプリ
		TextManager::player.set(u8R"(\b瓶の蓋を少しだけ開けてみた。\n心が安らぐような香りが広がった。\w9\e)");
		u.status->second.cool = 20;
		return false;
	}},
	{item_use + 304,[](Object& u) {	// きらめく石花
		TextManager::player.set(u8R"(\b花びらのような石にそっと触れた。\n壊してしまいそうですぐに手を離した。\w9\e)");
		u.status->second.cool = 20;
		return false;
	}},
	{item_use + 305,[](Object& u) {	// 花飾りの黒靴
		TextManager::player.set(u8R"(\b花の飾りが微かに揺れた。\n少し小さく、履くことはできないだろう。\w9\e)");
		u.status->second.cool = 20;
		return false;
	}},
	{enemy_action + 0,[](Object& u) {	// 前進
		auto i = Field::getIterator(u.pos, -u.status->second.speedFront, true);
		if(i == Field::cend())
		{
			u.pos -= u.status->second.speedFront;
		}
		else
		{
			if (!(**i) || (*i)->status->second.flag & 1)
			{
				u.pos -= u.status->second.speedFront;
				std::iter_swap(i, Field::getIterator(u.pos));
			}
			else
			{
				u.pos = (*i)->pos + 1;
			}
		}
		u.status->second.cool = 120;	// 仮
		return true;
	}},
};

std::unordered_map<int, Action::ValueDouble> Action::commonActionDouble =
{
	{killed,[](Object& u,Object& t) {	// 倒された
		TextManager::player.add(u8R"(\b重い衝撃を腹に喰らい、吹き飛ばされてその先で嘔吐く。\w4\n吐いた唾に血が混じった。\w2それが止まらない。\w6\n何度も吐いて、やがて内容物より血の占める割合の方が多くなった。\w4\n\nぐらり、視界が歪む。ぼやけた世界の中で影が目の前に迫る。\w6\n何かを振りかぶる。\w4\n……………………\w9\n\n（何れかのキーでタイトルへ戻る）\w9\w9\e)");
		return true;
	}},
	{item_use + 1,[](Object& u,Object&) {	// 話す
		TextManager::partner.set(u8R"(\bそれは言葉を返さない。\w9\e)");
		u.status->second.cool = 20;
		return false;
	}},
	{item_use + 2,[](Object& u,Object& t) {	// 殴る
		Effect::set(1,700,300,Effect::Pos::center);
		t.damage(static_cast<int>(u.status->second.atk* (u.has(Status::State::arousal) ? 1.3f : 1.0f)));
		u.status->second.cool = 60;
		return true;
	}},
	{item_use + 103,[](Object& u,Object& t) {	// 毒薬
		if((t.status->second.hp -= 20) <= 0)
			t.status->second.flag |= 4;
		TextManager::partner.set(u8R"(\b毒薬を投げつけ、20のダメージを与えた。\w9\e)");
		u.status->second.cool = 60;
		return true;
	}},
	{item_use + 200,[](Object& u,Object& t) {	// 錆びた短剣
		Effect::set(0,700,300,Effect::Pos::center);
		t.damage(static_cast<int>(u.status->second.atk* (u.has(Status::State::arousal) ? 1.3f : 1.0f)));
		u.status->second.cool = 40;
		return true;
	}},
	{item_use + 201,[](Object& u,Object& t) {	// ショートソード
		Effect::set(0,700,300,Effect::Pos::center);
		t.damage(static_cast<int>((u.status->second.atk * 1.1f + 6)* (u.has(Status::State::arousal) ? 1.3f : 1.0f)));
		u.status->second.cool = 35;
		return true;
	}},
	{item_use + 202,[](Object& u,Object& t) {	// ハンドアックス
		Effect::set(3,700,300,Effect::Pos::center);
		t.damage(static_cast<int>((u.status->second.atk * 1.8f + 1)* (u.has(Status::State::arousal) ? 1.3f : 1.0f)));
		u.status->second.cool = 70;
		return true;
	}},
	{item_use + 203,[](Object& u,Object& t) {	// ショートスピア
		Effect::set(2,700,300,Effect::Pos::center);
		t.damage(static_cast<int>((u.status->second.atk * 1.1f + 3)* (u.has(Status::State::arousal) ? 1.3f : 1.0f)));
		u.status->second.cool = 60;
		return true;
	}},
	{item_use + 204,[](Object& u,Object& t) {	// レイピア
		Effect::set(4,700,300,Effect::Pos::center);
		t.damage(static_cast<int>((u.status->second.atk * 1.5f + 9)* (u.has(Status::State::arousal) ? 1.3f : 1.0f)));
		u.status->second.cool = 35;
		return true;
	}},
	{item_use + 205,[](Object& u,Object& t) {	// バトルアックス
		Effect::set(3,700,300,Effect::Pos::center);
		t.damage(static_cast<int>((u.status->second.atk * 2.7f + 4)* (u.has(Status::State::arousal) ? 1.3f : 1.0f)));
		u.status->second.cool = 70;
		return true;
	}},
	{item_use + 206,[](Object& u,Object& t) {	// ハルバード
		Effect::set(6,700,300,Effect::Pos::center);
		t.damage(static_cast<int>((u.status->second.atk * 1.6f + 3)* (u.has(Status::State::arousal) ? 1.3f : 1.0f)));
		u.status->second.cool = 60;
		return true;
	}},
	{item_use + 207,[](Object& u,Object& t) {	// グラディウス
		Effect::set(0,700,300,Effect::Pos::center);
		t.damage(static_cast<int>((u.status->second.atk * 2.2f + 11) * (u.has(Status::State::arousal) ? 1.3f : 1.0f)));
		u.status->second.cool = 25;
		return true;
	}},
	{enemy_action + 0,[](Object& u,Object& t) {	// 攻撃
		if (t.status->first->type == Status::Type::player)
		{
			t.damage(static_cast<int>(u.status->second.atk* (u.has(Status::State::arousal) ? 1.3f : 1.0f)));
			if (t.status->second.flag & 4)
				u.execute(killed, t);
			Effect::set(10, 0, 0, Effect::Pos::leftup);
			Display::shake.set(1, 8, 6.0f);
		}
		else
		{
			u.pos -= u.status->second.speedFront;
			if (u.pos == t.pos)
				u.pos += 1;
			else if (u.pos < t.pos)
				std::iter_swap(Field::getIterator(u.pos), Field::getIterator(t.pos));
		}
		u.status->second.cool = 120;	// 仮
		return true;
	}},
};

