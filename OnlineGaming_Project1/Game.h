#pragma once

#include <SFML/Graphics.hpp>

#include <Player.h>
#include <BasePlayer.h>
#include <AuthPlayer.h>
#include <Server.h>

using namespace std;
using namespace sf;

enum class GameState
{
	Host,
	Join
};

class Game
{
public:
	Game(GameState t_state, string t_ip);
	~Game();
	void run();

	vector<Player*>* getPlayers();
	BasePlayer* getContainer();
	void setContainer(BasePlayer* t_container);
private:
	void update(Time t_deltaTime);
	void render();
	void events();
	void setup();

	sf::RenderWindow m_window;

	BasePlayer* m_container = nullptr;
	AuthPlayer* m_host = nullptr;

	vector<Player*>* m_players;

	int m_playerCount;
	GameState m_gameState;
	std::thread* m_lc;
};

