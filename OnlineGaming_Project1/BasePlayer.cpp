#include "BasePlayer.h"

BasePlayer::BasePlayer() : 
	m_radius(10.0f),
	m_position(Vector2f(0.0f,0.0f)),
	m_velocity(Vector2f(0.0f, 0.0f)),
	m_circleShape(make_shared<CircleShape>())
{
	setupBasePlayer();
}

BasePlayer::BasePlayer(Vector2f t_position, float t_radius) :
	m_radius(t_radius),
	m_position(t_position),
	m_velocity(Vector2f(0.0f, 0.0f)),
	m_circleShape(make_shared<CircleShape>())
{
	setupBasePlayer();
}

BasePlayer::~BasePlayer()
{
}

void BasePlayer::update(Time t_deltaTime)
{
	movement(t_deltaTime);
	screenWrap();
}

void BasePlayer::render(RenderWindow& t_window)
{
	t_window.draw(*m_circleShape);
}

void BasePlayer::setupBasePlayer()
{
	m_circleShape->setRadius(m_radius);
	m_circleShape->setPosition(m_position);
	m_circleShape->setFillColor(Color::Red);
	m_circleShape->setOrigin(Vector2f(m_radius, m_radius));
}

void BasePlayer::movement(Time t_deltaTime)
{
	//Movement in all directions
	if (Keyboard::isKeyPressed(sf::Keyboard::A) ||
		Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		m_velocity += Vector2f(-SPEED, 0.0f);
	}
	if (Keyboard::isKeyPressed(sf::Keyboard::S) ||
		Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		m_velocity += Vector2f(0.0f, SPEED);
	}
	if (Keyboard::isKeyPressed(sf::Keyboard::D) ||
		Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		m_velocity += Vector2f(SPEED, 0.0f);
	}
	if (Keyboard::isKeyPressed(sf::Keyboard::W) ||
		Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		m_velocity += Vector2f(0.0f, -SPEED);
	}

	m_position += t_deltaTime.asSeconds() * m_velocity;

	//sets the circles position
	m_circleShape->setPosition(m_position);

	//applies friction to player movement
	m_velocity *= RATE_OF_FRICTION;
}

void BasePlayer::screenWrap()
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
