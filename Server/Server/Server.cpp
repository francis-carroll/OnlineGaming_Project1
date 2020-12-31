#include "Server.h"

Server::Server(int t_port, bool t_broadcastPublically)
{
	WSAData m_wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &m_wsaData) != 0)
	{
		MessageBoxA(NULL, "Winsock startup failed", "Error", MB_OK | MB_ICONERROR);
		exit(0);
	}

	//public broadcasting
	if (t_broadcastPublically){
		m_address.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	//local broadcasting
	else {
		m_address.sin_addr.s_addr = inet_addr("127.0.0.1");
	}

	//sets the port
	m_address.sin_port = htons(t_port);
	//set the fam to IPV4
	m_address.sin_family = AF_INET;

	//create a socket
	m_sListen = socket(AF_INET, SOCK_STREAM, NULL);

	//binds the address to the socket
	if (bind(m_sListen, (SOCKADDR*)&m_address, m_addressLenght) == SOCKET_ERROR)
	{
		string errorMsg = "Failed to bind the address to the socket. Winsock error " + to_string(WSAGetLastError());
		MessageBoxA(NULL, errorMsg.c_str(), "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}

	//puts socket in listening state
	if (listen(m_sListen, SOMAXCONN) == SOCKET_ERROR)
	{
		string errorMsg = "Failed to listen for the address binded to the socket. Winsock error " + to_string(WSAGetLastError());
		MessageBoxA(NULL, errorMsg.c_str(), "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}

	serverPtr = this;
}

bool Server::listenForNewConnections()
{
	SOCKET newConnection = accept(m_sListen, (SOCKADDR*)&m_address, &m_addressLenght);
	if (newConnection == 0)
	{
		cout << "Failed to accept the clients connection" << endl;
		return false;
	}
	else {
		g_connections.push_back(make_shared<SOCKET>(newConnection));
		cout << "Client has been sucessfully conected" << endl;

		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandlerThread, (LPVOID)(g_connections.size() - 1), NULL, NULL);

		sendString(g_connections.size() - 1, Packet::P_ChatMessage, "Welcome!! This si the message of the day");
	}
}

bool Server::closeConnection(int t_id)
{
	if (closesocket(*g_connections.at(t_id)) == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSAENOTSOCK) return true;

		string errorMessage = "Failed to close the socket for client id: " + to_string(t_id) + " . Winsock error: " + to_string(WSAGetLastError()) + ".";
		MessageBoxA(NULL, errorMessage.c_str(), "Error", MB_OK | MB_ICONERROR);

		return false;
	}
	return true;
}

bool Server::ProcessPacket(int t_id, Packet t_packetType)
{
	switch (t_packetType)
	{
	case Packet::P_ChatMessage:
	{
		string message;
		if (!getString(t_id, message)) return false;

		if (g_connections.size() > 1)
		{
			int index = 0;
			for (shared_ptr<SOCKET> con : g_connections)
			{
				if (g_connections.at(t_id) != con)
				{
					if (!sendString(index, Packet::P_ChatMessage, message))
					{
						cout << "Failed to send message from client id: " << t_id << " to client id: " << index << endl;
					}
				}
				index++;
			}
			cout << "Chat message has been processed from client id: " << t_id << endl;
		}
		break;
	}
	case Packet::P_DataPacket:
		break;
	default:
		cout << "Unreconised Packet Type" << endl;
		break;
	}
	return true;
}

void Server::ClientHandlerThread(int t_id)
{
	Packet packetType;
	while (true)
	{
		if (!serverPtr->getPacketType(t_id, packetType)) break;

		if (!serverPtr->ProcessPacket(t_id, packetType)) break;
	}
	cout << "Lost connection to client id: " << t_id << endl;

	if (serverPtr->closeConnection(t_id))
	{
		cout << "Socket to the server has been closed." << endl;
	}
	else {
		cout << "Socket to the server cannot be closed." << endl;
	}
}

bool Server::sendAll(int t_id, char* t_data, int t_totalBytes)
{
	int bytesSent = 0;
	while (bytesSent < t_totalBytes)
	{
		int returnCheck = send(*g_connections.at(t_id), t_data + bytesSent, t_totalBytes - bytesSent, NULL);
		if (returnCheck == SOCKET_ERROR) return false;
		bytesSent += returnCheck;
	}
	return true;
}

bool Server::sendInt(int t_id, int t_int)
{
	if (!sendAll(t_id, (char*)&t_int, sizeof(int))) return false;
	return true;
}

bool Server::sendPacketType(int t_id, Packet t_packetType)
{
	if (!sendAll(t_id, (char*)&t_packetType, sizeof(Packet))) return false;
	return true;
}

bool Server::sendString(int t_id, Packet t_packetType, string t_string)
{
	//send the packet
	if (!sendPacketType(t_id, t_packetType)) return false;

	//send the message length
	int bufferlen = t_string.size();
	if (!sendInt(t_id, bufferlen)) return false;

	//send the string
	if (!sendAll(t_id, (char*)t_string.c_str(), bufferlen)) return false;
	return true;
}

bool Server::recvAll(int t_id, char* t_data, int t_totalBytes)
{
	int bytesRecieved = 0;
	while (bytesRecieved < t_totalBytes)
	{
		int returnCheck = recv(*g_connections.at(t_id), t_data + bytesRecieved, t_totalBytes - bytesRecieved, NULL);
		if (returnCheck == SOCKET_ERROR) return false;
		bytesRecieved += returnCheck;
	}
	return true;
}

bool Server::getInt(int t_id, int& t_int)
{
	if (!recvAll(t_id, (char*)&t_int, sizeof(int))) return false;
	return true;
}

bool Server::getPacketType(int t_id, Packet& t_packetType)
{
	if (!recvAll(t_id, (char*)&t_packetType, sizeof(Packet))) return false;
	return true;
}

bool Server::getString(int t_id, string& t_string)
{
	int bufferlen;
	if (!getInt(t_id, bufferlen)) return false;

	char* buffer = new char[bufferlen + 1];
	buffer[bufferlen] = '\0';

	if (!recvAll(t_id, buffer, bufferlen))
	{
		delete[] buffer;
		return false;
	}

	t_string = buffer;
	delete[] buffer;

	return true;
}
