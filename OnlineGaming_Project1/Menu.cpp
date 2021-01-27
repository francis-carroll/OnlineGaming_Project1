#include "Menu.h"
#include "Game.h"

Menu::Menu(Game* t_game) :
	m_game(t_game),
	m_host(Button(Vector2f(200.0f, 200.0f), "Host")),
	m_join(Button(Vector2f(300.0f, 200.0f), "Join")),
	m_confirm(Button(Vector2f(250.0f, 300.0f), "Confirm")),
	m_animateCurrent(true),
	m_animationTimer(seconds(0.0f)),
	m_inputContents("127.0.0.1")
{
	setup();
}

Menu::~Menu()
{
}

void Menu::update(Time t_dt)
{
	if (m_type == JoinType::None)
	{
		if (m_host.getButtonState() == ButtonState::Clicked)
		{
			m_type = JoinType::Host;
		}
		if (m_join.getButtonState() == ButtonState::Clicked)
		{
			m_type = JoinType::Join;
		}
	}
	else
	{
		if (m_confirm.getButtonState() == ButtonState::Clicked)
		{
			m_game->m_play = make_shared<Play>(m_type, m_inputContents);
			m_game->m_screen = Screen::Play;
		}

		animateCurrent(t_dt);
	}
}

void Menu::render(RenderWindow& t_window)
{
	if (m_type == JoinType::None)
	{
		m_host.render(t_window);
		m_join.render(t_window);
	}
	else
	{
		t_window.draw(m_inputText);
		m_confirm.render(t_window);
		t_window.draw(m_underline);
		t_window.draw(m_current);
	}
}

void Menu::handleInput(Event& t_event, RenderWindow& t_window)
{
	if (m_type == JoinType::None)
	{
		m_host.handleInput(t_event, t_window);
		m_join.handleInput(t_event, t_window);
	}
	else
	{
		textInput(t_event);
		m_confirm.handleInput(t_event, t_window);
	}
}

void Menu::setup()
{
	if (!m_font.loadFromFile("ASSETS/font.ttf")) { cout << "error loading font" << endl; }

	m_inputText.setFont(m_font);
	m_inputText.setString(m_inputContents);
	m_inputText.setCharacterSize(40.0f);
	m_inputText.setFillColor(Color::White);
	m_inputText.setPosition(Vector2f(250.0f, 200.0f));
	m_inputText.setOrigin(Vector2f(m_inputText.getGlobalBounds().width / 2.0f, m_inputText.getGlobalBounds().height / 2.0f));

	m_underline.setFillColor(Color::White);
	m_underline.setSize(Vector2f(300.f, 5.0f));
	m_underline.setOrigin(Vector2f(m_underline.getGlobalBounds().width / 2.0f, m_underline.getGlobalBounds().height / 2.0f));
	m_underline.setPosition(Vector2f(250.0f, 250.0f));

	m_current.setFillColor(Color::White);
	m_current.setSize(Vector2f(7.0f, 25.0f));
	m_current.setOrigin(Vector2f(m_current.getGlobalBounds().width / 2.0f, m_current.getGlobalBounds().height / 2.0f));
	m_current.setPosition(m_inputText.getPosition() + Vector2f(m_current.getGlobalBounds().width + (m_inputContents.size() * 8.0f), m_current.getGlobalBounds().height / 2.0f + 5.0f));
}

void Menu::textInput(Event& t_event)
{
	if (t_event.type == sf::Event::TextEntered)
	{
		if (t_event.text.unicode >= 48 && t_event.text.unicode <= 57 || t_event.text.unicode == 46)
		{
			m_inputContents += t_event.text.unicode;
		}
		else if (t_event.text.unicode == 8)
		{
			if (m_inputContents.length() != 0) {
				m_inputContents = m_inputContents.substr(0, m_inputContents.length() - 1);
			}
		}
		m_inputText.setString(m_inputContents);
		m_inputText.setPosition(Vector2f(250.0f, 200.0f));
		m_inputText.setOrigin(Vector2f(m_inputText.getGlobalBounds().width / 2.0f, m_inputText.getGlobalBounds().height / 2.0f));
		m_current.setOrigin(Vector2f(m_current.getGlobalBounds().width / 2.0f, m_current.getGlobalBounds().height / 2.0f));
		m_current.setPosition(m_inputText.getPosition() + Vector2f(m_current.getGlobalBounds().width + (m_inputContents.size() * 8.0f), m_current.getGlobalBounds().height / 2.0f + 5.0f));
	}
}

void Menu::animateCurrent(Time t_dt)
{
	m_animationTimer += t_dt;
	if (m_animationTimer.asSeconds() >= 0.01f)
	{
		if (m_animateCurrent)
		{
			m_current.setFillColor(m_current.getFillColor() - Color(0.0f, 0.0f, 0.0f, 5.0f));
			if (m_current.getFillColor().a <= 0)
			{
				m_animateCurrent = false;
			}
		}
		else
		{
			m_current.setFillColor(m_current.getFillColor() + Color(0.0f, 0.0f, 0.0f, 5.0f));
			if (m_current.getFillColor().a >= 255)
			{
				m_animateCurrent = true;
			}
		}
		m_animationTimer = seconds(0.0f);
	}
}
