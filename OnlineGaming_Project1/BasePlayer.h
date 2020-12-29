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

	void update(Time t_deltaTime);
	void render(RenderWindow& t_window);
private:
	void setupBasePlayer();
	void movement(Time t_deltaTime);
	void screenWrap();
protected:
	const float SPEED = 20.0f;
	const float RATE_OF_FRICTION = 0.95f;

	shared_ptr<CircleShape> m_circleShape;
	Vector2f m_position;
	Vector2f m_velocity;
	float m_radius;
};

