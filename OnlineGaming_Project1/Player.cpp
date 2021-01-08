#include "Player.h"

Player::Player(int t_id) : 
	m_circleShape(make_shared<CircleShape>()),
	m_position(Vector2f(100.0f, 100.0f)),
	m_id(t_id),
	m_radius(15.0f),
	m_identifier(Identifier::Visual),
	m_activePlayer(false),
	m_velocity(Vector2f(0.0f, 0.0f))
{
	setupPlayer();
}

Player::Player(int t_id, Identifier t_identifier) : 
	m_circleShape(make_shared<CircleShape>()),
	m_position(Vector2f(100.0f, 100.0f)),
	m_id(t_id),
	m_radius(15.0f),
	m_identifier(t_identifier),
	m_activePlayer(false),
	m_velocity(Vector2f(0.0f, 0.0f))
{
	setupPlayer();
}

Player::Player(int t_id, Vector2f t_position, float t_radius, Identifier t_identifier) :
	m_circleShape(make_shared<CircleShape>()),
	m_position(t_position),
	m_id(t_id),
	m_radius(t_radius),
	m_identifier(t_identifier),
	m_activePlayer(false),
	m_velocity(Vector2f(0.0f,0.0f))
{
	setupPlayer();
}

Player::~Player()
{
}

void Player::update(Time t_deltaTime)
{
	if (m_activePlayer)
	{
		screenWrap();
	}
}

void Player::render(RenderWindow& t_window)
{
	t_window.draw(*m_circleShape);
}

void Player::setActivePlayer(bool t_bool)
{
	m_activePlayer = t_bool;
}

void Player::setPosition(Vector2f t_position)
{
	m_position = t_position;
	m_circleShape->setPosition(m_position);
}

void Player::setID(int t_id)
{
	m_id = t_id;
}

void Player::setColor(Color t_color)
{
	m_circleShape->setFillColor(t_color);
}

Vector2f Player::getPosition()
{
	return m_position;
}

int Player::getID()
{
	return m_id;
}

Identifier Player::getIdentifier()
{
	return m_identifier;
}

Color Player::getColor()
{
	return m_circleShape->getFillColor();
}

void Player::setupPlayer()
{
	m_circleShape->setRadius(m_radius);
	m_circleShape->setPosition(m_position);
	//m_circleShape->setFillColor(Color::Red);
	m_circleShape->setOrigin(Vector2f(m_radius, m_radius));
}

void Player::screenWrap()
{
	//right side
	if (m_position.x - m_radius > 800.0f)
	{
		m_position = Vector2f(0.0f, m_position.y);
	}
	//left side
	if (m_position.x + m_radius < 0)
	{
		m_position = Vector2f(800.0f, m_position.y);
	}
	//top
	if (m_position.y + m_radius < 0)
	{
		m_position = Vector2f(m_position.x, 600.0f);
	}
	//bottom
	if (m_position.y - m_radius > 600.0f)
	{
		m_position = Vector2f(m_position.x, 0.0f);
	}
}
