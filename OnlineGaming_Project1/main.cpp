///@author Francis Carroll
///@date 08/12/20

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#ifdef _DEBUG 
#pragma comment(lib,"sfml-graphics-d.lib")
#pragma comment(lib,"sfml-audio-d.lib") 
#pragma comment(lib,"sfml-system-d.lib") 
#pragma comment(lib,"sfml-window-d.lib") 
#pragma comment(lib,"sfml-network-d.lib")
#else 
#pragma comment(lib,"sfml-graphics.lib") 
#pragma comment(lib,"sfml-audio.lib") 
#pragma comment(lib,"sfml-system.lib") 
#pragma comment(lib,"sfml-window.lib") 
#pragma comment(lib,"sfml-network.lib")
#endif

#pragma comment(lib,"ws2_32.lib")

#include <iostream>
#include <SFML/Graphics.hpp>
#include <WinSock2.h>
#include <Ws2tcpip.h>

#include "Game.h"

int main()
{
    srand((unsigned)time(nullptr));

    WSAData m_wsaData;
    WORD DllVersion = MAKEWORD(2, 1);
    if (WSAStartup(DllVersion, &m_wsaData) != 0)
    {
        MessageBoxA(NULL, "Winsock startup failed", "Error", MB_OK | MB_ICONERROR);
        exit(1);
    }

    SOCKADDR_IN m_address;
    int m_addressLength = sizeof(m_address);
    //sets the address
    m_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    //sets the port
    m_address.sin_port = htons(1111);
    //set the fam to IPV4
    m_address.sin_family = AF_INET;

    SOCKET m_connection = socket(AF_INET, SOCK_STREAM, NULL);
    if (connect(m_connection, (SOCKADDR*)&m_address, m_addressLength) != 0)
    {
        MessageBoxA(NULL, "Failed to connect", "Error", MB_OK | MB_ICONERROR);
        return 0;
    }
    cout << "Connected" << endl;

   /* Game m_game;

    m_game.run();*/

    char MOTD[256];
    recv(m_connection, MOTD, sizeof(MOTD), NULL);
    cout << "MOTD: " << MOTD << endl;

    system("pause");
    return 0;
}