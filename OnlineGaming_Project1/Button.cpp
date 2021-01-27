#include "Button.h"

Button::Button(Vector2f t_position, string t_text) :
	 m_position(t_position),
	m_textContents(t_text),
	m_padding(Vector2f(5.0f,5.0f)),
	m_state(ButtonState::None)
{
	setup();
}

Button::~Button()
{
}

void Button::update(Time t_dt)
{
}

void Button::render(RenderWindow& t_window)
{
	t_window.draw(m_button);
	t_window.draw(m_buttonText);
}

void Button::handleInput(Event& t_event, RenderWindow& t_window)
{
	hover(t_event, t_window);
	click(t_window);
}

ButtonState Button::getButtonState()
{
	return m_state;
}

void Button::setup()
{
	if (!m_font.loadFromFile("ASSETS/font.ttf")) { cout << "Error loading font" << endl; }

	m_buttonText.setFont(m_font);
	m_buttonText.setString(m_textContents);
	m_buttonText.setCharacterSize(25.0f);
	m_buttonText.setFillColor(Color::White);
	
	m_button.setSize(Vector2f(m_textContents.size() * (m_buttonText.getCharacterSize() / 2.0f) + m_padding.x, m_buttonText.getCharacterSize() + m_padding.y));
	m_button.setOrigin(m_button.getGlobalBounds().width / 2.0f, 0.0f);

	m_buttonText.setOrigin(m_buttonText.getGlobalBounds().width / 2.0f, m_buttonText.getGlobalBounds().height / 2.0f);
	m_buttonText.setPosition(m_position + Vector2f(0.0f,5.0f));

	m_button.setPosition(m_position);
	m_button.setOutlineColor(Color::White);
	m_button.setOutlineThickness(1.0f);
	m_button.setFillColor(Color::Black);
}

bool Button::collision(RenderWindow& t_window)
{
	Vector2i mousePos = sf::Mouse::getPosition(t_window);
	if (mousePos.x >= m_button.getPosition().x - (m_button.getSize().x / 2.0f) &&
		mousePos.y >= m_button.getPosition().y &&
		mousePos.x <= m_button.getPosition().x + (m_button.getSize().x / 2.0f) &&
		mousePos.y <= m_button.getPosition().y + m_button.getSize().y)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Button::hover(Event& t_event, RenderWindow& t_window)
{
	if (t_event.type == Event::MouseMoved)
	{
		if (collision(t_window))
		{
			m_button.setFillColor(Color(107, 107, 107));
			return true;
		}
		else
		{
			m_button.setFillColor(Color::Black);
			return false;
		}
	}
	return false;
}

bool Button::click(RenderWindow& t_window)
{
	if (Mouse::isButtonPressed(Mouse::Left) && collision(t_window))
	{
		m_buttonText.setFillColor(Color::Red);
		m_state = ButtonState::Clicked;
		return true;
	}
	else
	{
		m_buttonText.setFillColor(Color::White);
		m_state = ButtonState::None;
		return false;
	}
	return false;
}
