#include "Server.h"

Server::Server(int t_port, bool t_loopBacktoLocalHost)
{
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);

	if (WSAStartup(DllVersion, &wsaData) != 0)
	{
		MessageBoxA(0, "WinSock startup failed", "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}

	if (t_loopBacktoLocalHost)
		inet_pton(AF_INET, "127.0.0.1", &m_address.sin_addr.s_addr);
	else
		m_address.sin_addr.s_addr = htonl(INADDR_ANY); //loopback to any addr so others can connect

	m_address.sin_port = htons(t_port); //Port
	m_address.sin_family = AF_INET;

	m_sListen = socket(AF_INET, SOCK_STREAM, 0);

	if (bind(m_sListen, (SOCKADDR*)&m_address, sizeof(m_address)) == SOCKET_ERROR)
	{
		std::string ErrorMsg = "Failed to bind the address to our listening socket. Winsock Error:" + std::to_string(WSAGetLastError());
		MessageBoxA(0, ErrorMsg.c_str(), "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}

	if (listen(m_sListen, SOMAXCONN) == SOCKET_ERROR)
	{
		std::string ErrorMsg = "Failed to listen on listening socket. Winsock Error:" + std::to_string(WSAGetLastError());
		MessageBoxA(0, ErrorMsg.c_str(), "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}

	m_idCounter = 0;
	std::thread PST(packetSenderThread, ref(*this));
	PST.detach();
	m_threads.push_back(&PST);
}

Server::~Server()
{
	m_terminateThreads = true;
	for (std::thread* t : m_threads) //Wait for all created threads to end...
	{
		t->join();
	}
}

void Server::listenForNewConnections(Server& t_server)
{
	while(true)
	{
		if (t_server.m_activeConnection < 3)
		{
			listenForConnection(t_server);
		}
	}
}

bool Server::closeConnection(shared_ptr<Connection> t_connection)
{
	if (closesocket(t_connection->socket) == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSAENOTSOCK) return true;

		string errorMessage = "Failed to close the socket for client id: " + to_string(t_connection->id) + " . Winsock error: " + to_string(WSAGetLastError()) + ".";
		MessageBoxA(NULL, errorMessage.c_str(), "Error", MB_OK | MB_ICONERROR);

		return false;
	}
	return true;
}

void Server::addConnection(SOCKET& t_socket)
{
	m_connections.push_back(make_shared<Connection>(m_connections.size(), t_socket));
}

bool Server::listenForConnection(Server& t_server)
{
	int addrlen = sizeof(t_server.m_address);
	SOCKET newConnectionSocket = accept(t_server.m_sListen, (SOCKADDR*)&t_server.m_address, &addrlen); //Accept a new connection

	if (newConnectionSocket == 0) //If accepting the client connection failed
	{
		std::cout << "Failed to accept the client's connection." << std::endl;
		return false;
	}
	else //If client connection properly accepted
	{
		std::lock_guard<shared_mutex> lock(t_server.m_connectionManagerMutex);
		std::shared_ptr<Connection> newConnection(std::make_shared<Connection>(t_server.m_idCounter, newConnectionSocket));
		t_server.m_idCounter++;

		t_server.m_connections.push_back(newConnection);
		t_server.m_activeConnection++;
		std::cout << "Client Connected! ID:" << newConnection->id << std::endl;

		std::thread CHT(t_server.clientHandlerThread, std::ref(t_server), newConnection);
		CHT.detach();
		t_server.m_threads.push_back(&CHT);

		if (t_server.m_activeConnection != 1)
		{
			StateInfo updateState;
			updateState.m_gameState = State::Waiting;

			PS::StateUpdate is(updateState);
			newConnection->pm.append(is.toPacket(PacketType::RecieveState));
		}

		return true;
	}
}

int Server::getConnectionSize()
{
	return m_activeConnection;
}

void Server::initGame(int t_target)
{
	for (int i = 0; i < m_connections.size(); i++)
	{
		StartInfo temp;
		temp.target = t_target;
		temp.t_id = i;
		switch (i)
		{
		case 0:
			temp.playerType = ColorPlayer::Red;
			temp.pos = Vector2f(150.0f, 100.0f);
			break;
		case 1:
			temp.playerType = ColorPlayer::Yellow;
			temp.pos = Vector2f(400.0f, 200.0f);
			break;
		case 2:
			temp.playerType = ColorPlayer::Cyan;
			temp.pos = Vector2f(100.0f, 400.0f);
			break;
		default:
			break;
		};

		PS::StartUpdate is(temp);
		m_connections[i]->pm.append(is.toPacket(PacketType::SetupClient));
	}
}

bool Server::processPacket(shared_ptr<Connection> t_connection, PacketType t_packetType)
{
	switch (t_packetType)
	{
	case PacketType::UpdateRecv:
	{
		UpdateInfo temp;
		getUpdateInfo(t_connection, temp);

		PS::GameUpdate um(temp);
		std::shared_ptr<Packet> msgPacket = std::make_shared<Packet>(um.toPacket(PacketType::UpdateRecv)); //use shared_ptr instead of sending with SendString so we don't have to reallocate packet for each connection
		{
			std::shared_lock<std::shared_mutex> lock(m_connectionManagerMutex);
			for (shared_ptr<Connection> con : m_connections) //For each connection...
			{
				if (con->activeConnection)
				{
					if (con == t_connection) //If connection is the user who sent the message...
						continue;//Skip to the next user since there is no purpose in sending the message back to the user who sent it.
					con->pm.append(msgPacket);
				}
			}
		}
		break;
	}
	case PacketType::UpdateState:
	{
		StateInfo temp;
		getStateInfo(t_connection, temp);

		PS::StateUpdate um(temp);
		std::shared_ptr<Packet> msgPacket = std::make_shared<Packet>(um.toPacket(PacketType::RecieveState)); //use shared_ptr instead of sending with SendString so we don't have to reallocate packet for each connection
		{
			std::shared_lock<std::shared_mutex> lock(m_connectionManagerMutex);
			for (shared_ptr<Connection> con : m_connections) //For each connection...
			{
				if (con->activeConnection)
				{
					if (con == t_connection) //If connection is the user who sent the message...
						continue;//Skip to the next user since there is no purpose in sending the message back to the user who sent it.
					con->pm.append(msgPacket);
				}
			}
		}
		break;
	}
	case PacketType::SetupClient:
	{
		StartInfo temp;
		getStartInfo(t_connection, temp);

		PS::StartUpdate um(temp);
		std::shared_ptr<Packet> msgPacket = std::make_shared<Packet>(um.toPacket(PacketType::SetupVisuals)); //use shared_ptr instead of sending with SendString so we don't have to reallocate packet for each connection
		{
			std::shared_lock<std::shared_mutex> lock(m_connectionManagerMutex);
			for (shared_ptr<Connection> con : m_connections) //For each connection...
			{
				if (con->activeConnection)
				{
					if (con == t_connection) //If connection is the user who sent the message...
						continue;//Skip to the next user since there is no purpose in sending the message back to the user who sent it.
					con->pm.append(msgPacket);
				}
			}
		}
		break;
	}
	case PacketType::GameOver:
	{
		EndInfo temp;
		getEndInfo(t_connection, temp);

		PS::EndUpdate um(temp);
		std::shared_ptr<Packet> msgPacket = std::make_shared<Packet>(um.toPacket(PacketType::GameOver)); //use shared_ptr instead of sending with SendString so we don't have to reallocate packet for each connection
		{
			std::shared_lock<std::shared_mutex> lock(m_connectionManagerMutex);
			for (shared_ptr<Connection> con : m_connections) //For each connection...
			{
				if (con->activeConnection)
				{
					con->pm.append(msgPacket);
				}
			}
		}
		break;
	}
	case PacketType::ChatMessage:
	{
		std::string message; //string to store our message we received
		if (!getString(t_connection, message)) //Get the chat message and store it in variable: Message
			return false; //If we do not properly get the chat message, return false
						  //Next we need to send the message out to each user

		PS::ChatMessage cm(message);
		std::shared_ptr<Packet> msgPacket = std::make_shared<Packet>(cm.toPacket()); //use shared_ptr instead of sending with SendString so we don't have to reallocate packet for each connection
		{
			std::shared_lock<std::shared_mutex> lock(m_connectionManagerMutex);
			for (shared_ptr<Connection> con : m_connections) //For each connection...
			{
				if (con == t_connection) //If connection is the user who sent the message...
					continue;//Skip to the next user since there is no purpose in sending the message back to the user who sent it.
				con->pm.append(msgPacket);
			}
		}
		std::cout << "Processed chat message packet from user ID: " << t_connection->id << std::endl;
		break;
	}
	default:
		cout << "Unreconised Packet Type" << endl;
		break;
	}
	return true;
}

void Server::clientHandlerThread(Server& t_server, shared_ptr<Connection> t_connection)
{
	PacketType packettype;
	while (true)
	{
		if (t_server.m_terminateThreads) break;

		if (!t_server.getPacketType(t_connection, packettype)) break;

		if (!t_server.processPacket(t_connection, packettype)) break;
	}

	std::cout << "Lost connection to client ID: " << t_connection->id << std::endl;

	t_server.disconnectClient(t_connection);

	return;
}

void Server::packetSenderThread(Server& t_server)
{
	while (true)
	{
		if (t_server.m_terminateThreads == true)
			break;
		shared_lock<shared_mutex> lock(t_server.m_connectionManagerMutex);
		for (shared_ptr<Connection> con : t_server.m_connections) //for each connection...
		{
			if (con->pm.hasPendingPackets() && con->activeConnection) //If there are pending packets for this connection's packet manager
			{
				std::shared_ptr<Packet> p = con->pm.retrieve(); //Retrieve packet from packet manager
				if (!t_server.sendAll(con, (const char*)(&p->getBuffer()[0]), p->getBuffer().size())) //send packet to connection
				{
					std::cout << "Failed to send packet to ID: " << con->id << std::endl; //Print out if failed to send packet
					t_server.disconnectClient(con);
					break;
				}
			}
		}
		Sleep(5);
	}
	std::cout << "Ending Packet Sender Thread..." << std::endl;
}

void Server::disconnectClient(std::shared_ptr<Connection> t_connection)
{
	//lock_guard<shared_mutex> lock(m_connectionManagerMutex);

	t_connection->pm.clearPackets();
	closesocket(t_connection->socket);
	m_connections.erase(std::remove(m_connections.begin(), m_connections.end(), t_connection)); //Remove connection from vector of connections
	m_activeConnection--;
	std::cout << "Cleaned up client: " << t_connection->id << "." << std::endl;
	std::cout << "Total connections: " << m_connections.size() << std::endl;
}

bool Server::sendAll(std::shared_ptr<Connection> t_connection, const char* t_data, const int t_totalBytes)
{
	int bytesSent = 0;
	while (bytesSent < t_totalBytes)
	{
		int returnCheck = send(t_connection->socket, t_data + bytesSent, t_totalBytes - bytesSent, NULL);
		if (returnCheck == SOCKET_ERROR) return false;
		bytesSent += returnCheck;
	}
	return true;
}

bool Server::sendInt32_t(std::shared_ptr<Connection> t_connection, int32_t t_int)
{
	t_int = htonl(t_int);
	if (!sendAll(t_connection, (char*)&t_int, sizeof(int32_t))) return false;
	return true;
}

bool Server::sendPacketType(std::shared_ptr<Connection> t_connection, PacketType t_packetType)
{
	if (!sendInt32_t(t_connection, (int32_t)t_packetType)) return false;
	return true;
}

void Server::sendString(std::shared_ptr<Connection> t_connection, const string t_string)
{
	PS::ChatMessage message(t_string);
	t_connection->pm.append(message.toPacket());
}

void Server::sendUpdateInfo(std::shared_ptr<Connection> t_connection, const UpdateInfo t_data)
{
	PS::GameUpdate update(t_data);
	t_connection->pm.append(update.toPacket(PacketType::UpdateRecv));
}

bool Server::recvAll(std::shared_ptr<Connection> t_connection, char* t_data, int t_totalBytes)
{
	int bytesRecieved = 0;
	while (bytesRecieved < t_totalBytes)
	{
		int returnCheck = recv(t_connection->socket, t_data + bytesRecieved, t_totalBytes - bytesRecieved, NULL);
		if (returnCheck == SOCKET_ERROR) return false;
		bytesRecieved += returnCheck;
	}
	return true;
}

bool Server::getInt32_t(std::shared_ptr<Connection> t_connection, int32_t& t_int)
{
	if (!recvAll(t_connection, (char*)&t_int, sizeof(int32_t))) return false;
	t_int = ntohl(t_int);
	return true;
}

bool Server::getPacketType(std::shared_ptr<Connection> t_connection, PacketType& t_packetType)
{
	int packetType;
	if (!getInt32_t(t_connection, packetType)) return false;
	t_packetType = (PacketType)packetType;
	return true;
}

bool Server::getString(std::shared_ptr<Connection> t_connection, string& t_string)
{
	int32_t bufferlen;
	if (!getInt32_t(t_connection, bufferlen)) return false;

	char* buffer = new char[bufferlen + 1];
	buffer[bufferlen] = '\0';

	if (!recvAll(t_connection, buffer, bufferlen))
	{
		delete[] buffer;
		return false;
	}

	t_string = buffer;
	delete[] buffer;

	return true;
}

bool Server::getUpdateInfo(std::shared_ptr<Connection> t_connection, UpdateInfo& t_data)
{
	int32_t bufferlen;

	if (!getInt32_t(t_connection, bufferlen)) return false;

	if (bufferlen == 0) return 0;

	return recvAll(t_connection, (char*)&t_data, bufferlen);
}

bool Server::getStateInfo(shared_ptr<Connection> t_connection, StateInfo& t_data)
{
	int32_t bufferlen;

	if (!getInt32_t(t_connection, bufferlen)) return false;

	if (bufferlen == 0) return 0;

	return recvAll(t_connection, (char*)&t_data, bufferlen);
}

bool Server::getStartInfo(shared_ptr<Connection> t_connection, StartInfo& t_data)
{
	int32_t bufferlen;

	if (!getInt32_t(t_connection, bufferlen)) return false;

	if (bufferlen == 0) return 0;

	return recvAll(t_connection, (char*)&t_data, bufferlen);
}

bool Server::getEndInfo(shared_ptr<Connection> t_connection, EndInfo& t_data)
{
	int32_t bufferlen;

	if (!getInt32_t(t_connection, bufferlen)) return false;

	if (bufferlen == 0) return 0;

	return recvAll(t_connection, (char*)&t_data, bufferlen);
}
