#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib,"ws2_32.lib")

#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <iostream>

using namespace std;

class WinSocket
{
public:
	void start();
	SOCKET listenForConnection();
private:
	void startup();
	void initAddress();
	void createSocket();

	SOCKADDR_IN m_address;
	WSAData m_wsaData;
	WORD DllVersion;
	SOCKET m_sListen;
	SOCKET m_newConnection;
	int m_addressLength;
};

