#pragma once

#include <SFML/Graphics.hpp>

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
};

