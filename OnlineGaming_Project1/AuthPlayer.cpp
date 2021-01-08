#include "AuthPlayer.h"

AuthPlayer::AuthPlayer(int t_id) :
	BasePlayer(t_id, "127.0.0.1", Identifier::Host),
	m_server(make_shared<Server>(1111))
{
}

AuthPlayer::AuthPlayer(int t_id, Vector2f t_position, float t_radius) :
	BasePlayer(t_id, "127.0.0.1",  t_position, t_radius, Identifier::Host),
	m_server(make_shared<Server>(1111))
{
}

AuthPlayer::~AuthPlayer()
{
}

void AuthPlayer::update(Time t_deltaTime, vector<Player*>& t_players)
{
	BasePlayer::update(t_deltaTime);
	handleCollisions(t_players);
}

shared_ptr<Server> AuthPlayer::getServer()
{
	return m_server;
}

void AuthPlayer::handleCollisions(vector<Player*>& t_players)
{
	if (t_players.size() > 0)
	{
		//loop through other players
		for (Player* player : t_players)
		{
			if (player->getIdentifier() != Identifier::Host)
			{
				if (GLOBALS::distanceBetween(m_position, player->getPosition()) < 24.0f)
				{
					cout << "Player ID: " << m_id << " , Other Player: " << player->getID() << endl;
				}
			}
		}
	}
}