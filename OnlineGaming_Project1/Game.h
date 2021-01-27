#pragma once

#include <SFML/Graphics.hpp>

#include <Play.h>
#include <Menu.h>

using namespace std;
using namespace sf;

enum class Screen
{
	Menu, 
	Play
};

class Game
{
public:
	Game();
	~Game();
	void run();

	Screen m_screen = Screen::Menu;
	sf::RenderWindow m_window;
	shared_ptr<Play> m_play;
	shared_ptr<Menu> m_menu;
private:
	void update(Time t_deltaTime);
	void render();
	void events();
};