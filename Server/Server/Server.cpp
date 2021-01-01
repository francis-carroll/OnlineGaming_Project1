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

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)packetSenderThread, NULL, NULL, NULL);
}

bool Server::listenForNewConnections()
{
	SOCKET newConnectionSocket = accept(m_sListen, (SOCKADDR*)&m_address, &m_addressLenght);
	int newConnectionID = m_connections.size();
	if (newConnectionSocket == 0)
	{
		cout << "Failed to accept the clients connection" << endl;
		return false;
	}
	else {
		lock_guard<mutex> lock(m_connectionManagerMutex);

		if (m_unusedConnections > 0)
		{
			for (shared_ptr<Connection> con : m_connections)
			{
				if (!con->activeConnection)
				{
					con->socket = newConnectionSocket;
					con->activeConnection = true;
					newConnectionID = con->id;
					m_unusedConnections--;
					break;
				}
			}
		}
		else {
			addConnection(newConnectionSocket);
			cout << "Client has been sucessfully conected" << endl;
		}

		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)clientHandlerThread, (LPVOID)(newConnectionID), NULL, NULL);
	}
}

bool Server::closeConnection(int t_id)
{
	if (closesocket(m_connections[t_id]->socket) == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSAENOTSOCK) return true;

		string errorMessage = "Failed to close the socket for client id: " + to_string(t_id) + " . Winsock error: " + to_string(WSAGetLastError()) + ".";
		MessageBoxA(NULL, errorMessage.c_str(), "Error", MB_OK | MB_ICONERROR);

		return false;
	}
	return true;
}

void Server::addConnection(SOCKET& t_socket)
{
	m_connections.push_back(make_shared<Connection>(m_connections.size(), t_socket));
}

bool Server::processPacket(int t_id, PacketType t_packetType)
{
	switch (t_packetType)
	{
	case PacketType::ChatMessage:
	{
		string message;
		if (!getString(t_id, message)) return false;


		for (shared_ptr<Connection> con : m_connections)
		{
			if (!con->activeConnection) continue; //if the connection is not active, continue

			if (m_connections[t_id]->socket != con->socket)
			{
				sendString(con->id, message);
			}
		}

		cout << "Chat message has been processed from client id: " << t_id << endl;
		break;
	}
	case PacketType::FileTransferRequestFile:
	{
		string fileName;
		if (!getString(t_id, fileName)) return false;

		m_connections[t_id]->file.inFileStream.open(fileName, ios::binary | ios::ate);
		if (!m_connections[t_id]->file.inFileStream.is_open())
		{
			cout << "Client id: " << t_id << " requested file " << fileName << ", but that file does not exist";
			string errMsg = "Requested file " + fileName + " does not exist or was not found.";
			sendString(t_id, errMsg);
			return true;
		}

		m_connections[t_id]->file.fileName = fileName;
		m_connections[t_id]->file.fileSize = m_connections[t_id]->file.inFileStream.tellg();
		m_connections[t_id]->file.inFileStream.seekg(0);
		m_connections[t_id]->file.fileOffset = 0;

		if (!handlesSentFile(t_id)) return false;

		break;
	}
	case PacketType::FileTransferRequestNextBuffer:
	{
		if (!handlesSentFile(t_id)) return false;
		return true;
		break;
	}
	default:
		cout << "Unreconised Packet Type" << endl;
		break;
	}
	return true;
}

void Server::clientHandlerThread(int t_id)
{
	PacketType packetType;
	while (true)
	{
		if (!serverPtr->getPacketType(t_id, packetType)) break;

		if (!serverPtr->processPacket(t_id, packetType)) break;
	}

	serverPtr->disconnectClient(t_id);
}

void Server::packetSenderThread()
{
	while (true)
	{
		for (shared_ptr<Connection> con : serverPtr->m_connections)
		{
			if (con->pm.hasPendingPackets())
			{
				Packet p = con->pm.retrieve();
				if (!serverPtr->sendAll(con->id, p.getBuffer(), p.getSize()))
				{
					cout << "Failed to send packet to ID: " << con->id << endl;
				}
				delete p.getBuffer(); //clean up packet p
			}
		}
		Sleep(5);
	}
}

bool Server::handlesSentFile(int t_id)
{
	if (m_connections[t_id]->file.fileOffset >= m_connections[t_id]->file.fileSize) return true;

	if (!sendPacketType(t_id, PacketType::FileTransferByteBuffer)) return false;

	m_connections[t_id]->file.remainingBytes = m_connections[t_id]->file.fileSize - m_connections[t_id]->file.fileOffset;

	//file is finished sending
	if (m_connections[t_id]->file.remainingBytes > m_connections[t_id]->file.bufferSize)
	{
		m_connections[t_id]->file.inFileStream.read(m_connections[t_id]->file.buffer, m_connections[t_id]->file.bufferSize);

		if (!sendInt32_t(t_id, m_connections[t_id]->file.bufferSize)) return false;

		if(!sendAll(t_id, m_connections[t_id]->file.buffer, m_connections[t_id]->file.bufferSize)) return false;

		m_connections[t_id]->file.fileOffset += m_connections[t_id]->file.bufferSize;
	}
	else 
	{
		m_connections[t_id]->file.inFileStream.read(m_connections[t_id]->file.buffer, m_connections[t_id]->file.remainingBytes);

		if (!sendInt32_t(t_id, m_connections[t_id]->file.remainingBytes)) return false;

		if (!sendAll(t_id, m_connections[t_id]->file.buffer, m_connections[t_id]->file.remainingBytes)) return false;

		m_connections[t_id]->file.fileOffset += m_connections[t_id]->file.remainingBytes;
	}

	if (m_connections[t_id]->file.fileOffset == m_connections[t_id]->file.fileSize)
	{
		if (!sendPacketType(t_id, PacketType::FileTransfer_EndOfFile)) return false;

		cout << "File Sent: " << m_connections[t_id]->file.fileName << endl;
		cout << "File size(bytes): " << m_connections[t_id]->file.fileSize << endl;
	}

	return true;
}

void Server::disconnectClient(int t_id)
{
	lock_guard<mutex> lock(m_connectionManagerMutex);

	//if it is already disconnected
	if (!m_connections[t_id]->activeConnection) return;


	m_connections[t_id]->pm.clearPackets();
	m_connections[t_id]->activeConnection = false;

	if (serverPtr->closeConnection(t_id))
	{
		cout << "Socket to the server for client id: " << t_id  << " has been closed." << endl;
	}
	else {
		cout << "Socket to the server cannot be closed." << endl;
	}

	if (t_id == (m_connections.size() - 1))
	{
		m_connections.pop_back();

		for (int i = m_connections.size() - 1; i >= 0 && m_connections.size() > 1; i--)
		{
			if (m_connections[i]->activeConnection) break;
			m_connections.pop_back();
			m_unusedConnections--;
		}
	}
	else
	{
		m_unusedConnections++;
	}
}

bool Server::sendAll(int t_id, char* t_data, int t_totalBytes)
{
	int bytesSent = 0;
	while (bytesSent < t_totalBytes)
	{
		int returnCheck = send(m_connections.at(t_id)->socket, t_data + bytesSent, t_totalBytes - bytesSent, NULL);
		if (returnCheck == SOCKET_ERROR) return false;
		bytesSent += returnCheck;
	}
	return true;
}

bool Server::sendInt32_t(int t_id, int32_t t_int)
{
	t_int = htonl(t_int);
	if (!sendAll(t_id, (char*)&t_int, sizeof(int32_t))) return false;
	return true;
}

bool Server::sendPacketType(int t_id, PacketType t_packetType)
{
	if (!sendInt32_t(t_id, (int32_t)t_packetType)) return false;
	return true;
}

void Server::sendString(int t_id, string t_string)
{
	PS::ChatMessage message(t_string);
	m_connections[t_id]->pm.append(message.toPacket());
}

bool Server::recvAll(int t_id, char* t_data, int t_totalBytes)
{
	int bytesRecieved = 0;
	while (bytesRecieved < t_totalBytes)
	{
		int returnCheck = recv(m_connections[t_id]->socket, t_data + bytesRecieved, t_totalBytes - bytesRecieved, NULL);
		if (returnCheck == SOCKET_ERROR) return false;
		bytesRecieved += returnCheck;
	}
	return true;
}

bool Server::getInt32_t(int t_id, int32_t& t_int)
{
	if (!recvAll(t_id, (char*)&t_int, sizeof(int32_t))) return false;
	t_int = ntohl(t_int);
	return true;
}

bool Server::getPacketType(int t_id, PacketType& t_packetType)
{
	int packetType;
	if (!getInt32_t(t_id, packetType)) return false;
	t_packetType = (PacketType)packetType;
	return true;
}

bool Server::getString(int t_id, string& t_string)
{
	int32_t bufferlen;
	if (!getInt32_t(t_id, bufferlen)) return false;

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
