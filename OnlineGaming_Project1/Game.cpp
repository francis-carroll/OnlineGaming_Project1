#include "Game.h"

Game::Game() :
	m_window(sf::VideoMode(500, 500, 32), "Project 1", sf::Style::Default),
	m_menu(make_shared<Menu>(this)),
	m_play(nullptr)
{
}

Game::~Game()
{
}

void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	sf::Time timePerFrame = sf::seconds(1.f / 60.0f);

	while (m_window.isOpen())
	{
		events();
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			events();
			update(timePerFrame);
		}
		render();
	}
}

void Game::update(Time t_deltaTime)
{
	switch (m_screen)
	{
	case Screen::Menu:
		m_menu->update(t_deltaTime);
		break;
	case Screen::Play:
		m_play->update(t_deltaTime);
		break;
	}
}

void Game::render()
{
	m_window.clear();

	switch (m_screen)
	{
	case Screen::Menu:
		m_menu->render(m_window);
		break;
	case Screen::Play:
		m_play->render(m_window);
		break;
	}

	m_window.display();
}

void Game::events()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			m_window.close();
		}

		switch (m_screen)
		{
		case Screen::Menu:
			m_menu->handleInput(event, m_window);
			break;
		case Screen::Play:
			break;
		}
	}
}
