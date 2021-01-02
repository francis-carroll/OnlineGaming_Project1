#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib,"ws2_32.lib")

#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include <WinSock2.h>
#include <Ws2tcpip.h>

#include "Connection.h"
#include "GameUpdate.h"
#include "PacketStructs.h"

using namespace std;

class Client
{
public:
    Client(string t_ip, int t_port);
    bool connectSocket();
    bool closeConnection();
    
    //senders
    void sendUpdateInfo(UpdateInfo t_gameData);
    void sendString(string t_string);
private:
    bool processPacket(PacketType t_packetType);
    static void clientThread();
    static void packetSenderThread(Client* t_client);
    
    //sending
    bool sendInt32_t(int32_t t_int);
    bool sendAll(char* t_data, int t_totalBytes);
    bool sendPacketType(PacketType t_packetType);

    //getting
    bool recvAll(char* t_data, int t_totalBytes);
    bool getInt32_t(int32_t& t_int);
    bool getPacketType(PacketType& t_packetType);
    bool getString(string& t_string);
    bool getUpdateInfo(UpdateInfo& t_gameData);

    SOCKADDR_IN m_address;
    int m_addressLenght = sizeof(m_address);

    shared_ptr<Connection> m_connection;
};

static Client* clientPtr;