#include "Text.hpp"
#include <DxLib.h>
#include <boost/algorithm/string.hpp>
#include "convert_string.hpp"

Text TextManager::player(20, 300, 400, 280, 1.0f);
Text TextManager::partner(604, 300, 400, 280, 1.0f);

void Text::getElem(std::vector<std::u8string>& result, const std::u8string& input, size_t& off)const
{
	size_t ret;
	if (input[off] == '[')
		boost::split(result, input.substr(off + 1, ret = input.find_first_of(']', off) - off - 1), boost::is_any_of(","));
	else
	{
		result.clear();
		result.emplace_back(input.substr(off, ret = GetCharBytes(DX_CHARCODEFORMAT_UTF8, &input[off])));
	}
	off += ret;
}

void Text::set(const char8_t* v)
{
	text = v;
	count = -speed;
	textPos = 0;
}

void Text::update()
{
	if (textPos < text.size())
	{
		for (count += speed; quick || count >= 0.0f; quick || (count -= 1.0f))
		{
			if (text[textPos] == '\\')
			{
				switch (text[++textPos])
				{
				case 'q':	// 一括表示	次の\q、または一部の処理が出るまで
					quick ^= true;
					++textPos;
					break;
				case 'p':	// 一時停止	SPACE、64キー、矢印キーの入力を吸う　Shift、Ctrlは吸わない
					// まだ
					quick = false;
					++textPos;
					count = -1.0f;
					return;
				case 'b':	// メッセージウィンドウ表示
					// まだ
					++textPos;
					active = true;
					drawText.clear();
					break;
				case 'e':	// メッセージウィンドウ非表示
					// まだ
					++textPos;
					active = false;
					break;
				case 'w':	// ウェイト挿入
					quick = false;
					getElem(elem, text, ++textPos);
					count = std::stoi(ext::convert(elem.front())) * -10.0f;
					return;
				case 's':	// 速度変更
					getElem(elem, text, ++textPos);
					speed = std::stoi(ext::convert(elem.front())) * 0.2f;
					count = -1.0f;
					return;
				case 'n':	// 改行
					++textPos;
					drawText.append(u8"<br>");
					continue;
				}
			}
			else if (text[textPos] == '<')
			{
				for (size_t i = text.find_first_of('>', textPos) - textPos; i > 0; --i)
					drawText.push_back(text[textPos++]);
				continue;
			}
			for (int i = GetCharBytes(DX_CHARCODEFORMAT_UTF8, &text[textPos]); i > 0; --i)
				drawText.push_back(text[textPos++]);
			if (textPos >= text.size())
				break;
		}
	}
}

void Text::draw()const
{
	if (active)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 127);
		display.DrawBox(0, 0, display.siz, 0xff888888, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		display.DrawString(4, 4, drawText, 0xffffffff);
	}
}

