#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

#include <BasePlayer.h>
#include <Globals.h>
#include <Server.h>

using namespace std;
using namespace sf;

class AuthPlayer : public BasePlayer
{
public:
	AuthPlayer(int t_id);
	AuthPlayer(int t_id, Vector2f t_position, float t_radius);
	~AuthPlayer();

	void update(Time t_deltaTime, vector<Player*>& t_players);
	void updateState();
	shared_ptr<Server> getServer();
private:
	void handleCollisions(vector<Player*>& t_players);

	shared_ptr<Server> m_server;

	float m_gameTime;
};

