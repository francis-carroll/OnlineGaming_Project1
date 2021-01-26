#pragma once

#include <SFML/Graphics.hpp>

#include <Player.h>
#include <BasePlayer.h>
#include <AuthPlayer.h>
#include <Server.h>

using namespace std;
using namespace sf;

enum class JoinType
{
	Host,
	Join
};

class Game
{
public:
	Game(JoinType t_state, string t_ip);
	~Game();
	void run();

	vector<Player*>* getPlayers();
	BasePlayer* getContainer();
	void setContainer(BasePlayer* t_container);

	int m_winner;
	int m_target;
	sf::RenderWindow m_window;
private:
	void update(Time t_deltaTime);
	void render();
	void events();
	void setup();
	void setupText();
	void updatePlayers(Time t_dt);
	State getGameState();
	int getID();
	ColorPlayer getColor();
	int getTarget();

	BasePlayer* m_container = nullptr;
	AuthPlayer* m_host = nullptr;

	vector<Player*>* m_players;

	int m_playerCount;
	JoinType m_joinState;
	std::thread* m_lc;
	Text m_gameText;
	Font m_font;
	float m_timer;
};