#pragma once

#define NOMINMAX

#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

struct UpdateInfo
{
	int t_id;
	Color color;
	Vector2f pos;
};