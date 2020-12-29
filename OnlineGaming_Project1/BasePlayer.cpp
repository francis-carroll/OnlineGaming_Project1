#include "BasePlayer.h"

BasePlayer::BasePlayer() : 
	m_radius(10.0f),
	m_position(Vector2f(0.0f,0.0f)),
	m_velocity(Vector2f(0.0f, 0.0f))
{
	setupBasePlayer();
}

BasePlayer::BasePlayer(Vector2f t_position, float t_radius) :
	m_radius(t_radius),
	m_position(t_position),
	m_velocity(Vector2f(0.0f, 0.0f))
{
	setupBasePlayer();
}

BasePlayer::~BasePlayer()
{
}

void BasePlayer::update()
{
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
