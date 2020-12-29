#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

#include "BasePlayer.h"
#include "Globals.h"

using namespace std;
using namespace sf;

class AuthPlayer : public BasePlayer
{
public:
	AuthPlayer(int t_id);
	AuthPlayer(int t_id, Vector2f t_position, float t_radius);
	~AuthPlayer();

	void update(Time t_deltaTime, vector<shared_ptr<BasePlayer>>& t_players);
	void render(RenderWindow& t_window);
private:
	void handleCollisions(vector<shared_ptr<BasePlayer>>& t_players);
};

