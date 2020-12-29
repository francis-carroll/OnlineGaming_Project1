#pragma once

#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

namespace GLOBALS
{
	static float distanceBetween(Vector2f t_position1, Vector2f t_position2)
	{
		return std::sqrt(std::pow(t_position2.x - t_position1.x, 2.0f) + std::pow(t_position2.y - t_position1.y, 2.0f));
	}
}