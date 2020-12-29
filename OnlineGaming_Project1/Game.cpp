#include "Game.h"

Game::Game() :
	m_window(sf::VideoMode(800, 600, 32), "Project 1", sf::Style::Default),
	m_playerCount(0),
	m_host(make_shared<AuthPlayer>(m_playerCount, Vector2f(100.f,100.0f), 12.0f))
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
	m_host->update(t_deltaTime, m_players);

	for (shared_ptr<BasePlayer> player : m_players)
	{
		player->update(t_deltaTime);
	}
}

void Game::render()
{
	m_window.clear();

	m_host->render(m_window);

	for (shared_ptr<BasePlayer> player : m_players)
	{
		player->render(m_window);
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
	}
}

void Game::setup()
{
	m_host->setActivePlayer(true);
	
	for (int i = 0; i < 3; i++)
	{
		m_playerCount++;
		m_players.push_back(make_shared<BasePlayer>(m_playerCount, Vector2f((100.0f * i) + 200.0f, 100.0f), 12.0f));
	}
}
