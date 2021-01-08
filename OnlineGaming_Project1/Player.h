#pragma once

#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

enum class Identifier
{
	Visual,
	Client,
	Host
};

class Player
{
public:
	Player(int t_id);
	Player(int t_id, Identifier t_identifier);
	Player(int t_id, Vector2f t_position, float t_radius, Identifier t_identifier);
	~Player();

	void update(Time t_deltaTime);
	void render(RenderWindow& t_window);

	//setters
	void setActivePlayer(bool t_bool);
	void setPosition(Vector2f t_position);
	void setID(int t_id);
	void setColor(Color t_color);

	//getters
	Vector2f getPosition();
	int getID();
	Identifier getIdentifier();
	Color getColor();
private:
	void setupPlayer();
	void screenWrap();

protected:
	const float SPEED = 20.0f;
	const float RATE_OF_FRICTION = 0.95f;

	shared_ptr<CircleShape> m_circleShape;
	Vector2f m_position;
	Vector2f m_velocity;
	float m_radius;
	bool m_activePlayer;
	int m_id;

	Identifier m_identifier;
};

