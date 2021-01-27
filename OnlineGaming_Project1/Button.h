#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;
using namespace sf;

enum class ButtonState
{
	None,
	Clicked
};

class Button
{
public:
	Button(Vector2f t_position, string t_text);
	~Button();

	void update(Time t_dt);
	void render(RenderWindow& t_window);
	void handleInput(Event& t_event, RenderWindow& t_window);

	ButtonState getButtonState();
private:
	void setup();
	bool collision(RenderWindow& t_window);
	bool hover(Event& t_event, RenderWindow& t_window);
	bool click(RenderWindow& t_window);

	string m_textContents;
	Vector2f m_position;
	Vector2f m_padding;

	Text m_buttonText;
	Font m_font;

	RectangleShape m_button;
	ButtonState m_state;
};

