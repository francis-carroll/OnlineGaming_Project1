#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib,"ws2_32.lib")

#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include <WinSock2.h>
#include <Ws2tcpip.h>

#include "FileTransferData.h"
#include "PacketType.h"

using namespace std;

class Client
{
public:
    Client(string t_ip, int t_port);
    bool connectSocket();
    bool closeConnection();
    bool SendString(PacketType t_packetType, string t_string, bool t_includePacketType = true);

    bool requestFile(string t_fileName);
private:
    bool processPacket(PacketType t_packetType);
    static void clientThread();
    
    //sending
    bool sendAll(char* t_data, int t_totalBytes);
    bool sendInt32_t(int32_t t_int);
    bool sendPacketType(PacketType t_packetType);

    //getting
    bool recvAll(char* t_data, int t_totalBytes);
    bool getInt32_t(int32_t& t_int);
    bool getPacketType(PacketType& t_packetType);
    bool getString(string& t_string);

    SOCKET m_connection;
    SOCKADDR_IN m_address;
    int m_addressLenght = sizeof(m_address);

    FileTransferData m_file;
};

static Client* clientPtr;