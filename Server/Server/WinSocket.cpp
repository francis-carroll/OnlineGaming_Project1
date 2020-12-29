#include "WinSocket.h"

void WinSocket::start()
{
    startup();
    initAddress();
    createSocket();
}

void WinSocket::startup()
{
    DllVersion = MAKEWORD(2, 1);
    if (WSAStartup(DllVersion, &m_wsaData) != 0)
    {
        MessageBoxA(NULL, "Winsock startup failed", "Error", MB_OK | MB_ICONERROR);
        exit(1);
    }
}

void WinSocket::initAddress()
{
    //assign size of address
    m_addressLength = sizeof(m_address);
    //sets the address
    m_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    //sets the port
    m_address.sin_port = htons(1111);
    //set the fam to IPV4
    m_address.sin_family = AF_INET;
}

void WinSocket::createSocket()
{
    //create a socket
    m_sListen = socket(AF_INET, SOCK_STREAM, NULL);
    //binds the address to the socket
    bind(m_sListen, (SOCKADDR*)&m_address, m_addressLength);
    //puts socket in listening state
    listen(m_sListen, SOMAXCONN);
}

SOCKET WinSocket::listenForConnection()
{
    SOCKET newConnection = accept(m_sListen, (SOCKADDR*)&m_address, &m_addressLength);
    if (newConnection == 0)
    {
        return NULL;
    }
    else {
        return newConnection;
    }
}
