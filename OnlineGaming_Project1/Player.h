#pragma once

#include <SFML/Graphics.hpp>
#include <GameUpdate.h>

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
	void setColor(ColorPlayer t_color);
	void setColorPlayer(ColorPlayer t_color);
	void setColorSingle(ColorPlayer t_color);
	void setState(State t_state);
	void setTarget(int t_target);

	//getters
	Vector2f getPosition();
	int getID();
	Identifier getIdentifier();
	Color getColor();
	ColorPlayer getColorPlayer();
	State getState();
	int getTarget();
private:
	void setupPlayer();
	void screenWrap();
	void boundry();

protected:
	const float SPEED = 20.0f;
	const float RATE_OF_FRICTION = 0.95f;

	shared_ptr<CircleShape> m_circleShape;
	Vector2f* m_position;
	Vector2f m_velocity;
	float m_radius;
	bool m_activePlayer;
	int m_id;

	Identifier m_identifier;
	State m_gameState;
	ColorPlayer m_color;
	int m_target;
};

