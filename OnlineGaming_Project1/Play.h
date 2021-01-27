#pragma once

#include <SFML/Graphics.hpp>
#include <Player.h>
#include <BasePlayer.h>
#include <AuthPlayer.h>
#include <Server.h>

using namespace std;
using namespace sf;

class Play
{
public:
	Play(JoinType t_type, string t_ip);
	~Play();

	void update(Time t_dt);
	void render(RenderWindow& t_window);

	vector<Player*>* getPlayers();
	BasePlayer* getContainer();
	void setContainer(BasePlayer* t_container);

	int m_winner;
	int m_target;
	float m_gameTime;
private:
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
	string m_ip;
	float m_time;
};

