#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib,"ws2_32.lib")

#include "PacketStructs.h"
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <iostream>
#include <vector>
#include <string>
#include <shared_mutex>
#include "Connection.h"

using namespace std;
using namespace sf;

class Server
{
public:
	Server(int t_port, bool t_loopBacktoLocalHost = true);
	~Server();
	static void listenForNewConnections(Server& t_server);
	static bool listenForConnection(Server& t_server);
	int getConnectionSize();
	void initGame(int t_target);
private:
	bool processPacket(std::shared_ptr<Connection> t_connection, PacketType t_packetType);
	static void clientHandlerThread(Server& t_server, shared_ptr<Connection> t_connection);
	static void packetSenderThread(Server& t_server);
	void disconnectClient(shared_ptr<Connection> t_connection);
	bool closeConnection(shared_ptr<Connection> t_connection);
	void addConnection(SOCKET& t_socket);

	//sending
	bool sendAll(shared_ptr<Connection> t_connection, const char* t_data, const int t_totalBytes);
	bool sendInt32_t(shared_ptr<Connection> t_connection, int32_t t_int);
	bool sendPacketType(shared_ptr<Connection> t_connection, PacketType t_packetType);
	void sendString(shared_ptr<Connection> t_connection, const string t_string);
	void sendUpdateInfo(shared_ptr<Connection> t_connection, const UpdateInfo t_data);

	//getting
	bool recvAll(shared_ptr<Connection> t_connection, char* t_data, int t_totalBytes);
	bool getInt32_t(shared_ptr<Connection> t_connection, int32_t& t_int);
	bool getPacketType(shared_ptr<Connection> t_connection, PacketType& t_packetType);
	bool getString(shared_ptr<Connection> t_connection, string& t_string);
	bool getUpdateInfo(shared_ptr<Connection> t_connection, UpdateInfo& t_data);
	bool getStateInfo(shared_ptr<Connection> t_connection, StateInfo& t_data);
	bool getEndInfo(shared_ptr<Connection> t_connection, EndInfo& t_data);

	vector<shared_ptr<Connection>> m_connections;
	SOCKADDR_IN m_address;
	bool m_terminateThreads = false;
	std::vector<std::thread*> m_threads;
	SOCKET m_sListen;
	shared_mutex m_connectionManagerMutex;
	int m_idCounter = 0;
	int m_activeConnection = 0;
};