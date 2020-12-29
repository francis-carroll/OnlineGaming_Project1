///@author Francis Carroll
///@date 29/12/20

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib,"ws2_32.lib")

#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <iostream>
#include "WinSocket.h"

using namespace std;

int main()
{
    WinSocket m_socket;
    m_socket.start();

    SOCKET m_newConnection = m_socket.listenForConnection();
    if (m_newConnection == NULL)
    {
        cout << "Failed to connect to client" << endl;
    }
    else {
        char MOTD[256] = "WELCOME!!";
        send(m_newConnection, MOTD, sizeof(MOTD), NULL);
    }

    system("pause");
    return 0;
}