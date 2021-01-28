#include "Play.h"

Play::Play(JoinType t_type, string t_ip) : 
    m_playerCount(0),
    m_joinState(t_type),
    m_players(new vector<Player*>()),
	m_ip(t_ip),
	m_time(0.0f)
{
    setup();
    if (m_host != nullptr)
    {
        std::thread lc(m_host->getServer()->listenForNewConnections, ref(*m_host->getServer()));
        lc.detach();
        m_lc = &lc;
    }
}

Play::~Play()
{
}

void Play::update(Time t_dt)
{
	if (m_host != nullptr)
	{
		if (m_host->getServer()->getConnectionSize() >= 3 && m_host->getState() == State::Waiting)
		{
			m_host->setPrevState(m_host->getState());
			m_host->setState(State::Init);
			m_target = std::rand() % 3;
			m_host->setTarget(m_target);
			m_host->getServer()->initGame(m_target);
			m_timer = 0.0f;
		}
		else if (m_host->getServer()->getConnectionSize() >= 3 && m_host->getState() == State::Init)
		{
			m_timer += t_dt.asSeconds();
			if (m_timer >= 10.0f)
			{
				m_host->setPrevState(m_host->getState());
				m_host->setState(State::Playing);
			}
		}
		else if (m_host->getServer()->getConnectionSize() < 3 && m_host->getState() == State::Playing ||
			m_host->getServer()->getConnectionSize() < 3 && m_host->getState() == State::GameOver ||
			m_host->getServer()->getConnectionSize() < 3 && m_host->getState() == State::Init)
		{
			m_host->setPrevState(m_host->getState());
			m_host->setState(State::Waiting);
		}
	}

	if (getGameState() == State::Init)
	{
		if (getID() == getTarget())
		{
			m_gameText.setString("                  You Are " + ToString::colorToString(getColor()) + ". \n Run from the other players so \n           they dont catch you");
			m_gameText.setPosition(Vector2f(70.0f, 200.0f));
		}
		else
		{
			m_gameText.setString("            You Are " + ToString::colorToString(getColor()) + ". \n Catch " + ToString::colorToString(static_cast<ColorPlayer>(getTarget())) + " to win the game.");
			m_gameText.setPosition(Vector2f(70.0f, 200.0f));
		}
	}
	else if (getGameState() == State::Playing)
	{
		updatePlayers(t_dt);
	}
	else if (getGameState() == State::Waiting)
	{
		m_gameText.setString("Waiting for Players");
		m_gameText.setPosition(Vector2f(140.0f, 200.0f));
	}
	else if (getGameState() == State::GameOver)
	{
		if (getID() == m_winner)
		{
			m_gameText.setString("You Won in " + to_string(m_gameTime) + " seconds.");
			m_gameText.setPosition(Vector2f(100.0f, 200.0f));
		}
		else if (getID() == m_target)
		{
			m_gameText.setString("           You Got Caught, \nYou Lasted " + to_string(m_gameTime) + " seconds.");
			m_gameText.setPosition(Vector2f(100.0f, 200.0f));
		}
		else if (getID() != m_target && getID() != m_winner)
		{
			m_gameText.setString("You Lost in " + to_string(m_gameTime) + " seconds.");
			m_gameText.setPosition(Vector2f(100.0f, 200.0f));
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

void Play::render(RenderWindow& t_window)
{
	if (getGameState() == State::Playing)
	{
		for (Player* player : *m_players)
		{
			player->render(t_window);
		}
	}
	else if (getGameState() == State::Waiting || getGameState() == State::GameOver || getGameState() == State::Init)
	{
		t_window.draw(m_gameText);
	}
}

vector<Player*>* Play::getPlayers()
{
    return m_players;
}

BasePlayer* Play::getContainer()
{
    return m_container;
}

void Play::setContainer(BasePlayer* t_container)
{
    m_container = t_container;
}

void Play::setup()
{
	for (int i = 0; i < 3; i++)
	{
		m_players->push_back(new Player(i));
		m_players->at(i)->setPosition(Vector2f(-100.0f, -100.0f));
	}

	if (m_joinState == JoinType::Host)
	{
		AuthPlayer* host = new AuthPlayer(m_playerCount, Vector2f(100.0f, 100.0f), 15.0f, m_ip);
		host->setActivePlayer(true);
		m_players->at(0) = host;
		host->setupClient(this);
		host->getServer()->listenForConnection(*host->getServer());
		m_host = host;
		m_container = nullptr;
		m_players->at(0)->setState(State::Waiting);
	}
	else {
		BasePlayer* temp = new BasePlayer(-1, m_ip, Vector2f(150.0f, 100.0f), 15.0f);
		temp->setActivePlayer(true);
		temp->setupClient(this);
		m_host = nullptr;
		m_container = temp;
		m_container->setState(State::None);
	}
	setupText();
}

void Play::setupText()
{
	if (!m_font.loadFromFile("ASSETS/font.ttf")) cout << "font not loaded" << endl;

	m_gameText.setFont(m_font);
	m_gameText.setCharacterSize(30);
	m_gameText.setPosition(Vector2f(50.0f, 150.0f));
	m_gameText.setString("");
}

void Play::updatePlayers(Time t_dt)
{
	m_time += t_dt.asSeconds();
	for (Player* player : *m_players)
	{
		if (player->getIdentifier() == Identifier::Host)
		{
			AuthPlayer* temp = static_cast<AuthPlayer*>(player);
			temp->update(t_dt, *m_players);

			if (m_time >= 0.03f)
			{
				UpdateInfo updateData;
				updateData.t_id = temp->getID();
				updateData.pos = temp->getPosition();

				PS::GameUpdate is(updateData);
				temp->getClient()->getClientConnection()->pm.append(is.toPacket(PacketType::UpdateRecv));
			}
		}
		else if (player->getIdentifier() == Identifier::Client) {
			BasePlayer* temp = static_cast<BasePlayer*>(player);
			temp->update(t_dt);

			if (m_time >= 0.03f)
			{
				UpdateInfo updateData;
				updateData.t_id = temp->getID();
				updateData.pos = temp->getPosition();

				PS::GameUpdate is(updateData);
				temp->getClient()->getClientConnection()->pm.append(is.toPacket(PacketType::UpdateRecv));
			}
		}
		else {
			player->update(t_dt);
		}
	}
	if (m_time >= 0.03f)
	{
		m_time = 0.0f;
	}
}

State Play::getGameState()
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

int Play::getID()
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

ColorPlayer Play::getColor()
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

int Play::getTarget()
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
