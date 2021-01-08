#include "BasePlayer.h"
#include <Game.h>

BasePlayer::BasePlayer(int t_id, string t_ip) :
	Player(t_id, Identifier::Client)
{
}

BasePlayer::BasePlayer(int t_id, string t_ip, Identifier t_identifier) :
	Player(t_id, t_identifier)
{
}

BasePlayer::BasePlayer(int t_id, string t_ip, Vector2f t_position, float t_radius) :
	Player(t_id, t_position, t_radius, Identifier::Client)
{
}

BasePlayer::BasePlayer(int t_id, string t_ip, Vector2f t_position, float t_radius, Identifier t_identifier)   :
	Player(t_id, t_position, t_radius, t_identifier)
{
}

BasePlayer::~BasePlayer()
{
}

void BasePlayer::update(Time t_deltaTime)
{
	if (m_client->getClientConnection()->activeConnection && m_activePlayer)
	{
		movement(t_deltaTime);
		Player::update(t_deltaTime);
	}
}

void BasePlayer::setupClient(Game* t_game)
{
	m_client = make_shared<Client>("127.0.0.1", 1111);
	if (!m_client->connectSocket(t_game))
	{
		cout << "Failed to connect to server" << endl;
	}
}

shared_ptr<Client> BasePlayer::getClient()
{
	return m_client;
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
