#include "AuthPlayer.h"

AuthPlayer::AuthPlayer(int t_id, string t_ip) :
	BasePlayer(t_id, t_ip, Identifier::Host),
	m_server(make_shared<Server>(8000)),
	m_gameTime(0.0f)
{
}

AuthPlayer::AuthPlayer(int t_id, Vector2f t_position, float t_radius, string t_ip) :
	BasePlayer(t_id, t_ip,  t_position, t_radius, Identifier::Host),
	m_server(make_shared<Server>(8000)),
	m_gameTime(0.0f)
{
}

AuthPlayer::~AuthPlayer()
{
}

void AuthPlayer::update(Time t_deltaTime, vector<Player*>& t_players)
{
	m_gameTime += t_deltaTime.asSeconds();
	BasePlayer::update(t_deltaTime);
	handleCollisions(t_players);
}

void AuthPlayer::updateState()
{
	if (m_gameState != m_prevGameState)
	{
		m_prevGameState = m_gameState;

		sendState();
	}
}

shared_ptr<Server> AuthPlayer::getServer()
{
	return m_server;
}

void AuthPlayer::sendState()
{
	StateInfo updateState;
	updateState.m_gameState = m_gameState;

	PS::StateUpdate is(updateState);
	getClient()->getClientConnection()->pm.append(is.toPacket(PacketType::UpdateState));
}

void AuthPlayer::handleCollisions(vector<Player*>& t_players)
{
	if (t_players.size() > 0)
	{
		//loop through other players
		for (Player* player : t_players)
		{
			if (player != t_players.at(m_target))
			{
				if (GLOBALS::distanceBetween(player->getPosition(), t_players.at(m_target)->getPosition()) <= 24.0f)
				{
					cout << "Target ID: " << t_players.at(m_target)->getID() << " , Collided Player: " << player->getID() << endl;

					m_prevGameState = m_gameState;
					m_gameState = State::GameOver;

					EndInfo endInfo;
					endInfo.target = m_target;
					endInfo.winner = player->getID();
					endInfo.gameTime = m_gameTime;

					PS::EndUpdate end(endInfo);
					m_client->getClientConnection()->pm.append(end.toPacket(PacketType::GameOver));
				}
			}
		}
	}
}