#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib,"ws2_32.lib")

#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include <WinSock2.h>
#include <Ws2tcpip.h>

using namespace std;

enum class Packet
{
    P_ChatMessage,
    P_DataPacket
};

class Client
{
public:
    Client(string t_ip, int t_port);
    bool connectSocket();
    bool closeConnection();
    bool SendString(Packet t_packetType, string t_string);
private:
    bool processPacket(Packet t_packetType);
    static void clientThread();
    
    //sending
    bool sendAll(char* t_data, int t_totalBytes);
    bool sendInt(int t_int);
    bool sendPacketType(Packet t_packetType);

    //getting
    bool recvAll(char* t_data, int t_totalBytes);
    bool getInt(int& t_int);
    bool getPacketType(Packet& t_packetType);
    bool getString(string& t_string);

    SOCKET m_connection;
    SOCKADDR_IN m_address;
    int m_addressLenght = sizeof(m_address);
};

static Client* clientPtr;