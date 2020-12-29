///@author Francis Carroll
///@date 29/12/20

#pragma comment(lib,"ws2_32.lib")

#include <iostream>
#include <WinSock2.h>

using namespace std;

int main()
{
    //server setup of winsocket
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
    m_address.sin_addr.s_addr = InetPton(AF_INET, "127.0.0.1", &m_address);
    //sets the port
    m_address.sin_port = htons(1111);
    //set the fam to IPV4
    m_address.sin_family = AF_INET;

    //create a socket
    SOCKET m_sListen = socket(AF_INET, SOCK_STREAM, NULL);
    //binds the address to the socket
    bind(m_sListen, (SOCKADDR*)&m_address, m_addressLength);
    //puts socket in listening state
    listen(m_sListen, SOMAXCONN);

    SOCKET m_newConnection;
    m_newConnection = accept(m_sListen, (SOCKADDR*)&m_address, &m_addressLength);
    if (m_newConnection == 0)
    {
        cout << "Failed to accept the client connection" << endl;
    }
    else {
        cout << "Client Connected" << endl;
        /*char MOTD[256] = "WELCOME!!";
        send(m_newConnection, MOTD, sizeof(MOTD), NULL);*/
    }

    system("pause");
    return 0;
}