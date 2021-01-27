#pragma once

#include <SFML/Graphics.hpp>
#include <Button.h>
#include <Globals.h>

class Game;

using namespace std;
using namespace sf;

class Menu
{
public:
	Menu(Game* t_game);
	~Menu();

	void update(Time t_dt);
	void render(RenderWindow& t_window);
	void handleInput(Event& t_event, RenderWindow& t_window);
private:
	void setup();
	void textInput(Event& t_event);
	void animateCurrent(Time t_dt);

	Game* m_game;

	Button m_host;
	Button m_join;
	Button m_confirm;

	Font m_font;
	Text m_inputText;
	string m_inputContents;

	JoinType m_type = JoinType::None;

	RectangleShape m_underline;
	RectangleShape m_current;
	bool m_animateCurrent;
	Time m_animationTimer;
};

