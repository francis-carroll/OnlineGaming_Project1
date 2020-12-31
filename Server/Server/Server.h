#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib,"ws2_32.lib")

#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

enum class Packet
{
    P_ChatMessage,
    P_DataPacket
};

class Server
{
public:
    Server(int t_port, bool t_broadcastPublically = false);
	bool listenForNewConnections();

	bool closeConnection(int t_id);
private:
	bool ProcessPacket(int t_id, Packet t_packetType);
	static void ClientHandlerThread(int t_id);

	//sending
	bool sendAll(int t_id, char* t_data, int t_totalBytes);
	bool sendInt(int t_id, int t_int);
	bool sendPacketType(int t_id, Packet t_packetType);
	bool sendString(int t_id, Packet t_packetType, string t_string);

	//getting
	bool recvAll(int t_id, char* t_data, int t_totalBytes);
	bool getInt(int t_id, int& t_int);
	bool getPacketType(int t_id, Packet& t_packetType);
	bool getString(int t_id, string& t_string);

	vector<shared_ptr<SOCKET>> g_connections;
	SOCKADDR_IN m_address;
	int m_addressLenght = sizeof(m_address);
	SOCKET m_sListen;
};

static Server* serverPtr;