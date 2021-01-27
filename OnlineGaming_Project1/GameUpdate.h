#pragma once

#define NOMINMAX

#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

enum class State
{
	None,
	Waiting,
	Playing,
	GameOver,
	Init
};

enum class ColorPlayer
{
	Red,
	Yellow,
	Cyan
};

struct StartInfo
{
	int t_id;
	Vector2f pos;
	ColorPlayer playerType;
	int target;
};

struct UpdateInfo
{
	int t_id;
	Vector2f pos;
};

struct StateInfo
{
	State m_gameState;
};

struct EndInfo
{
	int target;
	int winner;
	float gameTime;
};

class ToString
{
public:
	static string colorToString(ColorPlayer t_color)
	{
		switch (t_color)
		{
		case ColorPlayer::Red:
		{
			return "Red";
			break;
		}
		case ColorPlayer::Yellow:
		{
			return "Yellow";
			break;
		}
		case ColorPlayer::Cyan:
		{
			return "Blue";
			break;
		}
		}
	}
};