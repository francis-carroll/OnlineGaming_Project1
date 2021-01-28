#include "Player.h"

Player::Player(int t_id) : 
	m_circleShape(make_shared<CircleShape>()),
	m_position(new Vector2f(100.0f, 100.0f)),
	m_id(t_id),
	m_radius(15.0f),
	m_identifier(Identifier::Visual),
	m_activePlayer(false),
	m_velocity(Vector2f(0.0f, 0.0f)),
	m_gameState(State::Waiting),
	m_prevGameState(State::None)
{
	setupPlayer();
}

Player::Player(int t_id, Identifier t_identifier) : 
	m_circleShape(make_shared<CircleShape>()),
	m_position(new Vector2f(100.0f, 100.0f)),
	m_id(t_id),
	m_radius(15.0f),
	m_identifier(t_identifier),
	m_activePlayer(false),
	m_velocity(Vector2f(0.0f, 0.0f)),
	m_gameState(State::Waiting),
	m_prevGameState(State::None)
{
	setupPlayer();
}

Player::Player(int t_id, Vector2f t_position, float t_radius, Identifier t_identifier) :
	m_circleShape(make_shared<CircleShape>()),
	m_position(new Vector2f(t_position)),
	m_id(t_id),
	m_radius(t_radius),
	m_identifier(t_identifier),
	m_activePlayer(false),
	m_velocity(Vector2f(0.0f,0.0f)),
	m_gameState(State::Waiting),
	m_prevGameState(State::None)
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
		//screenWrap();
		boundry();
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
	*m_position = t_position;
	m_circleShape->setPosition(*m_position);
}

void Player::setID(int t_id)
{
	m_id = t_id;
}

void Player::setColor(ColorPlayer t_color)
{
	switch (t_color)
	{
	case ColorPlayer::Red:
		m_circleShape->setFillColor(Color::Red);
		break;
	case ColorPlayer::Yellow:
		m_circleShape->setFillColor(Color::Yellow);
		break;
	case ColorPlayer::Cyan:
		m_circleShape->setFillColor(Color::Cyan);
		break;
	default:
		m_circleShape->setFillColor(Color::White);
		break;
	}
}

void Player::setColorPlayer(ColorPlayer t_color)
{
	m_color = t_color;
}

void Player::setColorSingle(ColorPlayer t_color)
{
	switch (t_color)
	{
	case ColorPlayer::Red:
		m_circleShape->setFillColor(Color::Red);
		break;
	case ColorPlayer::Yellow:
		m_circleShape->setFillColor(Color::Yellow);
		break;
	case ColorPlayer::Cyan:
		m_circleShape->setFillColor(Color::Cyan);
		break;
	default:
		m_circleShape->setFillColor(Color::White);
		break;
	}
}

void Player::setState(State t_state)
{
	m_gameState = t_state;
}

void Player::setPrevState(State t_state)
{
	m_prevGameState = t_state;
}

void Player::setTarget(int t_target)
{
	m_target = t_target;
}

void Player::setVelocity(Vector2f t_velocity)
{
	m_velocity = t_velocity;
}

Vector2f Player::getPosition()
{
	return *m_position;
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

ColorPlayer Player::getColorPlayer()
{
	return m_color;
}

State Player::getState()
{
	return m_gameState;
}

State Player::getPrevState()
{
	return m_prevGameState;
}

int Player::getTarget()
{
	return m_target;
}

void Player::setupPlayer()
{
	m_circleShape->setRadius(m_radius);
	m_circleShape->setPosition(*m_position);
	//m_circleShape->setFillColor(Color::Red);
	m_circleShape->setOrigin(Vector2f(m_radius, m_radius));
}

void Player::screenWrap()
{
	//right side
	if (m_position->x - m_radius > 500.0f)
	{
		m_position = new Vector2f(0.0f, m_position->y);
	}
	//left side
	if (m_position->x + m_radius < 0)
	{
		m_position = new Vector2f(500.0f, m_position->y);
	}
	//top
	if (m_position->y + m_radius < 0)
	{
		m_position = new Vector2f(m_position->x, 500.0f);
	}
	//bottom
	if (m_position->y - m_radius > 500.0f)
	{
		m_position = new Vector2f(m_position->x, 0.0f);
	}
}

void Player::boundry()
{
	if (m_position->x - m_radius < 0)
	{
		m_position->x = m_radius;
	}

	else if (m_position->x + m_radius > 500)
	{
		m_position->x = 500 - m_radius;
	}

	if (m_position->y - m_radius < 0)
	{
		m_position->y = m_radius;
	}

	else if (m_position->y + m_radius > 500)
	{
		m_position->y = 500 - m_radius;
	}

	m_circleShape->setPosition(*m_position);
}
