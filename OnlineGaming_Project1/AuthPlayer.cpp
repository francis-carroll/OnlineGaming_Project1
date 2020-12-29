#include "AuthPlayer.h"

AuthPlayer::AuthPlayer(int t_id) :
	BasePlayer(t_id)
{
}

AuthPlayer::AuthPlayer(int t_id, Vector2f t_position, float t_radius) :
	BasePlayer(t_id, t_position, t_radius)
{
}

AuthPlayer::~AuthPlayer()
{
}

void AuthPlayer::update(Time t_deltaTime, vector<shared_ptr<BasePlayer>>& t_players)
{
	BasePlayer::update(t_deltaTime);
	handleCollisions(t_players);
}

void AuthPlayer::render(RenderWindow& t_window)
{
	BasePlayer::render(t_window);
}

void AuthPlayer::handleCollisions(vector<shared_ptr<BasePlayer>>& t_players)
{
	if (t_players.size() > 0)
	{
		//loop through other players
		for (shared_ptr<BasePlayer> player : t_players)
		{
			if (GLOBALS::distanceBetween(m_position, player->getPosition()) < 24.0f)
			{
				cout << "Player ID: " << m_id << " , Other Player: " << player->getID() << endl;
			}
		}
	}
}