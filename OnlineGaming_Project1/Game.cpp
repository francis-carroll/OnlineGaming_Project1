#include "Game.h"

Game::Game(JoinType t_state, string t_ip) :
	m_window(sf::VideoMode(500, 500, 32), "Project 1", sf::Style::Default),
	m_playerCount(0),
	m_joinState(t_state), 
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
	if (m_host != nullptr)
	{
		if (m_host->getServer()->getConnectionSize() >= 3 && m_host->getState() == State::Waiting)
		{
			m_host->setState(State::Init);
			m_target = std::rand() % 3;
			m_host->setTarget(m_target);
			m_host->getServer()->initGame(m_target);
			m_timer = 0.0f;
		}
		else if (m_host->getState() == State::Init)
		{
			m_timer += t_deltaTime.asSeconds();
			if (m_timer >= 10.0f)
			{
				m_host->setState(State::Playing);
			}
		}
		else if(m_host->getServer()->getConnectionSize() < 3 && m_host->getState() == State::Playing ||
			m_host->getServer()->getConnectionSize() < 3 && m_host->getState() == State::GameOver)
		{
			m_host->setState(State::Waiting);
		}
	}

	if (getGameState() == State::Init)
	{
		if (getID() == getTarget())
		{
			m_gameText.setString("You Are " + ToString::colorToString(getColor()) + ". \n Run from the other players so they \n dont catch you");
		}
		else
		{
			m_gameText.setString("You Are " + ToString::colorToString(getColor()) + ". \n Catch " + ToString::colorToString(static_cast<ColorPlayer>(getTarget())) + " to win the game.");
		}
	}
	else if (getGameState() == State::Playing)
	{
		updatePlayers(t_deltaTime);
	}
	else if (getGameState() == State::Waiting)
	{
		m_gameText.setString("Waiting for Players");
	}
	else if (getGameState() == State::GameOver)
	{
		if (getID() == m_winner)
		{
			m_gameText.setString("You Won");
		}
		else if (getID() == m_target)
		{
			m_gameText.setString("You Got Caught");
		}
		else if (getID() != m_target && getID() != m_winner)
		{
			m_gameText.setString("You Lost");
		}
	}

	for (Player* player : *m_players)
	{
		if (player->getIdentifier() == Identifier::Host)
		{
			AuthPlayer* temp = static_cast<AuthPlayer*>(player);
			temp->updateState();
		}
	}
}

void Game::render()
{
	m_window.clear();

	if (getGameState() == State::Playing)
	{
		for (Player* player : *m_players)
		{
			player->render(m_window);
		}
	}
	else if (getGameState() == State::Waiting || getGameState() == State::GameOver || getGameState() == State::Init)
	{
		m_window.draw(m_gameText);
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
		m_players->at(i)->setPosition(Vector2f(-100.0f, -100.0f));
	}

	if (m_joinState == JoinType::Host)
	{
		AuthPlayer* host = new AuthPlayer(m_playerCount, Vector2f(100.0f, 100.0f), 15.0f);
		host->setActivePlayer(true);
		m_players->at(0) = host;
		host->setupClient(this);
		host->getServer()->listenForConnection(*host->getServer());
		m_host = host;
		m_container = nullptr;
		m_players->at(0)->setState(State::Waiting);
	}
	else {
		BasePlayer* temp = new BasePlayer(-1, "127.0.0.1", Vector2f(150.0f, 100.0f), 15.0f);
		temp->setActivePlayer(true);
		temp->setupClient(this);
		m_host = nullptr;
		m_container = temp;
		m_container->setState(State::None);
	}
	setupText();
}

void Game::setupText()
{
	if (!m_font.loadFromFile("ASSETS/font.ttf")) cout << "font not loaded" << endl;

	m_gameText.setFont(m_font);
	m_gameText.setCharacterSize(30);
	m_gameText.setPosition(Vector2f(50.0f, 150.0f));
	m_gameText.setString("");
}

void Game::updatePlayers(Time t_dt)
{
	for (Player* player : *m_players)
	{
		if (player->getIdentifier() == Identifier::Host)
		{
			AuthPlayer* temp = static_cast<AuthPlayer*>(player);
			temp->update(t_dt, *m_players);

			UpdateInfo updateData;
			updateData.t_id = temp->getID();
			updateData.pos = temp->getPosition();
			updateData.playerType = temp->getColorPlayer();

			PS::GameUpdate is(updateData);
			temp->getClient()->getClientConnection()->pm.append(is.toPacket(PacketType::UpdateRecv));
		}
		else if (player->getIdentifier() == Identifier::Client) {
			BasePlayer* temp = static_cast<BasePlayer*>(player);
			temp->update(t_dt);

			UpdateInfo updateData;
			updateData.t_id = temp->getID();
			updateData.playerType = temp->getColorPlayer();
			updateData.pos = temp->getPosition();

			PS::GameUpdate is(updateData);
			temp->getClient()->getClientConnection()->pm.append(is.toPacket(PacketType::UpdateRecv));
		}
		else {
			player->update(t_dt);
		}
	}
}

State Game::getGameState()
{
	if (m_joinState == JoinType::Host)
	{
		return m_host->getState();
	}
	else
	{
		return m_container->getState();
	}
}

int Game::getID()
{
	if (m_joinState == JoinType::Host)
	{
		return m_host->getID();
	}
	else
	{
		return m_container->getID();
	}
}

ColorPlayer Game::getColor()
{
	if (m_joinState == JoinType::Host)
	{
		return m_host->getColorPlayer();
	}
	else
	{
		return m_container->getColorPlayer();
	}
}

int Game::getTarget()
{
	if (m_joinState == JoinType::Host)
	{
		return m_host->getTarget();
	}
	else
	{
		return m_container->getTarget();
	}
}
