#include "Game.h"

Game::Game(GameState t_state, string t_ip) :
	m_window(sf::VideoMode(800, 600, 32), "Project 1", sf::Style::Default),
	m_playerCount(0),
	m_gameState(t_state), 
	m_players(new vector<Player*>())
{
	setup();
}

Game::~Game()
{
}

void Game::run()
{
	if (m_host != nullptr)
	{
		std::thread lc(m_host->getServer()->listenForNewConnections, ref(*m_host->getServer()));
		lc.detach();
		m_lc = &lc;
	}

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

vector<Player*>* Game::getPlayers()
{
	return m_players;
}

BasePlayer* Game::getContainer()
{
	return m_container;
}

void Game::setContainer(BasePlayer* t_container)
{
	m_container = nullptr;
}

void Game::update(Time t_deltaTime)
{
	for (Player* player : *m_players)
	{
		if (player->getIdentifier() == Identifier::Host)
		{
			AuthPlayer* temp = static_cast<AuthPlayer*>(player);
			temp->update(t_deltaTime, *m_players);

			UpdateInfo updateData;
			updateData.t_id = temp->getID();
			updateData.color = temp->getColor();
			updateData.pos = temp->getPosition();

			PS::GameUpdate is(updateData);
			temp->getClient()->getClientConnection()->pm.append(is.toPacket(PacketType::UpdateRecv));
		}
		else if (player->getIdentifier() == Identifier::Client){
			BasePlayer* temp = static_cast<BasePlayer*>(player);
			temp->update(t_deltaTime);

			UpdateInfo updateData;
			updateData.t_id = temp->getID();
			updateData.color = temp->getColor();
			updateData.pos = temp->getPosition();

			PS::GameUpdate is(updateData);
			temp->getClient()->getClientConnection()->pm.append(is.toPacket(PacketType::UpdateRecv));
		}
		else {
			player->update(t_deltaTime);
		}
	}
}

void Game::render()
{
	m_window.clear();

	for (Player* player : *m_players)
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
	for (int i = 0; i < 3; i++)
	{
		m_players->push_back(new Player(i));
	}

	if (m_gameState == GameState::Host)
	{
		AuthPlayer* host = new AuthPlayer(m_playerCount, Vector2f(100.0f, 100.0f), 15.0f);
		host->setActivePlayer(true);
		m_players->at(0) = host;
		host->setupClient(this);
		host->getServer()->listenForConnection(*host->getServer());
		m_host = host;
		m_container = nullptr;
	}
	else {
		BasePlayer* temp = new BasePlayer(-1, "127.0.0.1", Vector2f(150.0f, 100.0f), 15.0f);
		temp->setActivePlayer(true);
		temp->setupClient(this);
		m_host = nullptr;
		m_container = temp;
	}
}
