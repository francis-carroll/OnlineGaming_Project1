#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib,"ws2_32.lib")

#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <iostream>
#include <vector>
#include <string>
#include <mutex>

#include "Connection.h"
#include "PacketStructs.h"

using namespace std;

class Server
{
public:
    Server(int t_port, bool t_broadcastPublically = false);
	bool listenForNewConnections();

	bool closeConnection(int t_id);
	void addConnection(SOCKET& t_socket);
private:
	bool processPacket(int t_id, PacketType t_packetType);
	static void clientHandlerThread(int t_id);
	static void packetSenderThread();
	bool handlesSentFile(int t_id);
	void disconnectClient(int t_id);

	//sending
	bool sendAll(int t_id, char* t_data, int t_totalBytes);
	bool sendInt32_t(int t_id, int32_t t_int);
	bool sendPacketType(int t_id, PacketType t_packetType);
	void sendString(int t_id, string t_string);

	//getting
	bool recvAll(int t_id, char* t_data, int t_totalBytes);
	bool getInt32_t(int t_id, int32_t& t_int);
	bool getPacketType(int t_id, PacketType& t_packetType);
	bool getString(int t_id, string& t_string);

	vector<shared_ptr<Connection>> m_connections;
	SOCKADDR_IN m_address;
	int m_addressLenght = sizeof(m_address);
	SOCKET m_sListen;

	mutex m_connectionManagerMutex;
	int m_unusedConnections = 0;
};

static Server* serverPtr;