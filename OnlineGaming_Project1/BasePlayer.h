#pragma once

#include <Client.h>
#include <Player.h>

class Game;

class BasePlayer : public Player
{
public:
	BasePlayer(int t_id, string t_ip);
	BasePlayer(int t_id, string t_ip, Identifier t_identifier);
	BasePlayer(int t_id, string t_ip, Vector2f t_position, float t_radius);
	BasePlayer(int t_id, string t_ip, Vector2f t_position, float t_radius, Identifier t_identifier);
	~BasePlayer();

	void setupClient(Game* t_game);
	void update(Time t_deltaTime);

	shared_ptr<Client> getClient();
protected:
	void movement(Time t_deltaTime);
	shared_ptr<Client> m_client;
};