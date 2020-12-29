#include "Game.h"

Game::Game() :
	m_window(sf::VideoMode(800, 600, 32), "Project 1", sf::Style::Default),
	p(BasePlayer(Vector2f(100.0f,100.0f), 10.0f))
{
	setup();
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
	p.update(t_deltaTime);
}

void Game::render()
{
	m_window.clear();

	p.render(m_window);

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
	}
}

void Game::setup()
{
}
