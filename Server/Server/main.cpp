///@author Francis Carroll
///@date 29/12/20

#define NOMINMAX
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib,"ws2_32.lib")

#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <iostream>
#include <string>

#include "Server.h"

using namespace std;

int main()
{
    Server server(1111);

    for (int i = 0; i < 100; i++)
    {
        server.listenForNewConnections();
    }

    system("pause");
    return 0;
}