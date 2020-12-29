#pragma once

#include <SFML/Graphics.hpp>

#include "BasePlayer.h"
#include "AuthPlayer.h"

using namespace std;
using namespace sf;

class Game
{
public:
	Game();
	~Game();
	void run();
private:
	void update(Time t_deltaTime);
	void render();
	void events();
	void setup();

	sf::RenderWindow m_window;

	shared_ptr<AuthPlayer> m_host;

	vector<shared_ptr<BasePlayer>> m_players;

	int m_playerCount;
};

