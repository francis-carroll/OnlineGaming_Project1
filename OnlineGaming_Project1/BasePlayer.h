#pragma once

#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class BasePlayer
{
public:
	BasePlayer();
	BasePlayer(Vector2f t_position ,float t_radius);
	~BasePlayer();

	void update();
	void render(RenderWindow& t_window);
private:
	void setupBasePlayer();
protected:
	shared_ptr<CircleShape> m_circleShape;
	Vector2f m_position;
	Vector2f m_velocity;
	float m_radius;
};

