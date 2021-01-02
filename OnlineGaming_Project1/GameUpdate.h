#pragma once

#define NOMINMAX

#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;


enum class State {
	Yes, NO
};

struct UpdateInfo
{
	Color color;
	Vector2f pos;
	State state;
};