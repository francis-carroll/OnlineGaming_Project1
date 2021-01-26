#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib,"ws2_32.lib")

#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <shared_mutex>

#include "Connection.h"
#include "GameUpdate.h"
#include "PacketStructs.h"

class Player;
class BasePlayer;
class Game;

using namespace std;

class Client
{
public:
    Client(string t_ip, int t_port);
    ~Client();
    bool connectSocket(Game* t_game);
    bool closeConnection();
    
    //senders
    void sendUpdateInfo(UpdateInfo t_gameData);
    void sendString(string t_string);

    shared_ptr<Connection> getClientConnection();
private:
    bool processPacket(PacketType t_packetType, Game* t_game);
    static void clientThread(Client& t_client, Game* t_game);
    static void packetSenderThread(Client& t_client);
    void disconnectClient(shared_ptr<Connection> t_connection);
    
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
    bool getStateInfo(StateInfo& t_data);
    bool getStartInfo(StartInfo& t_data);
    bool getEndInfo(EndInfo& t_data);

    bool m_terminateThreads = false;
    SOCKADDR_IN m_address;
    int m_addressLenght = sizeof(m_address);

    shared_ptr<Connection> m_connection;
    std::vector<std::thread*> m_threads;
    shared_mutex m_connectionManagerMutex;
};