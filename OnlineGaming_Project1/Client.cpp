#include "Client.h"

Client::Client(string t_ip, int t_port)
{
    WSAData m_wsaData;
    WORD DllVersion = MAKEWORD(2, 1);
    if (WSAStartup(DllVersion, &m_wsaData) != 0)
    {
        MessageBoxA(NULL, "Winsock startup failed", "Error", MB_OK | MB_ICONERROR);
        exit(0);
    }

    //sets the address
    m_address.sin_addr.s_addr = inet_addr(t_ip.c_str());
    //sets the port
    m_address.sin_port = htons(t_port);
    //set the fam to IPV4
    m_address.sin_family = AF_INET;

    clientPtr = this;
}

bool Client::connectSocket()
{
    m_connection = socket(AF_INET, SOCK_STREAM, NULL);
    if (connect(m_connection, (SOCKADDR*)&m_address, m_addressLenght) != 0)
    {
        MessageBoxA(NULL, "Failed to connect", "Error", MB_OK | MB_ICONERROR);
        return false;
    }
    CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)clientThread, NULL, NULL, NULL);
    return true;
}

bool Client::closeConnection()
{
    if (closesocket(m_connection) == SOCKET_ERROR)
    {
        if (WSAGetLastError() == WSAENOTSOCK) return true;

        string errorMessage = "Failed to close the socket. Winsock error: " + to_string(WSAGetLastError()) + ".";
        MessageBoxA(NULL, errorMessage.c_str(), "Error", MB_OK | MB_ICONERROR);

        return false;
    }
    return true;
}

bool Client::processPacket(PacketType t_packetType)
{
    switch (t_packetType)
    {
    case PacketType::ChatMessage:
    {
        string message;
        if (!getString(message)) return false;
        cout << message << endl;
        break;
    }
    case PacketType::FileTransferByteBuffer:
    {
        int32_t bufferSize;
        if (!getInt32_t(bufferSize)) return false;

        if (!recvAll(m_file.buffer, bufferSize)) return false;

        m_file.outFileStream.write(m_file.buffer, bufferSize);
        m_file.bytesWritten += bufferSize;

        cout << "Recieved byte buffer for file transfer of size: " << bufferSize << endl;

        sendPacketType(PacketType::FileTransferRequestNextBuffer);

        break;
    }
    case PacketType::FileTransfer_EndOfFile:
    {
        cout << "File transfer completed. File recieved." << endl;
        cout << "File Name: " << m_file.fileName << endl;
        cout << "File Size(bytes): " << m_file.bytesWritten << std::endl;

        m_file.bytesWritten = 0;
        m_file.outFileStream.close();
        break;
    }
    default:
        cout << "Unreconised Packet Type" << endl;
        break;
    }
    return true;
}

void Client::clientThread()
{
    PacketType packetType;
    while (true)
    {
        if (!clientPtr->getPacketType(packetType)) break;

        if (!clientPtr->processPacket(packetType)) break;
    }
    cout << "Lost connection to the server" << endl;

    //close the connection
    if (clientPtr->closeConnection())
    {
        cout << "Socket to the server has been closed." << endl;
    }
    else {
        cout << "Socket to the server cannot be closed." << endl;
    }
}

bool Client::sendAll(char* t_data, int t_totalBytes)
{
    int bytesSent = 0;
    while (bytesSent < t_totalBytes)
    {
        int returnCheck = send(m_connection, t_data + bytesSent, t_totalBytes - bytesSent, NULL);
        if (returnCheck == SOCKET_ERROR) return false;
        bytesSent += returnCheck;
    }
    return true;
}

bool Client::sendInt32_t(int32_t t_int)
{
    t_int = htonl(t_int);
    if (!sendAll((char*)&t_int, sizeof(int32_t))) return false;

    return true;
}

bool Client::recvAll(char* t_data, int t_totalBytes)
{
    int bytesRecieved = 0;
    while (bytesRecieved < t_totalBytes)
    {
        int returnCheck = recv(m_connection, t_data + bytesRecieved, t_totalBytes - bytesRecieved, NULL);
        if (returnCheck == SOCKET_ERROR) return false;
        bytesRecieved += returnCheck;
    }
    return true;
}

bool Client::getInt32_t(int32_t& t_int)
{
    if (!recvAll((char*)&t_int, sizeof(int32_t))) return false;
    t_int = ntohl(t_int);
    return true;
}

bool Client::sendPacketType(PacketType t_packetType)
{
    if (!sendInt32_t((int32_t)t_packetType)) return false;
    return true;
}

bool Client::getPacketType(PacketType& t_packetType)
{
    int packetType;
    if (!getInt32_t(packetType)) return false;
    t_packetType = (PacketType)packetType;
    return true;
}

bool Client::SendString(PacketType t_packetType, string t_string, bool t_includePacketType)
{
    if (t_includePacketType)
    {
        //send the packet
        if (!sendPacketType(t_packetType)) return false;
    }

    //send the message length
    int32_t bufferlen = t_string.size();
    if (!sendInt32_t(bufferlen)) return false;

    //send the string
    if (!sendAll((char*)t_string.c_str() , bufferlen)) return false;
    return true;
}

bool Client::requestFile(string t_fileName)
{
    m_file.outFileStream.open(t_fileName);
    m_file.fileName = t_fileName;
    m_file.bytesWritten = 0;
    if (!m_file.outFileStream.is_open())
    {
        cout << "ERROR: Function(Client::RequestFile" << endl;
        return false;
    }

    cout << "Requesting file from server: " << t_fileName << endl;

    if (!sendPacketType(PacketType::FileTransferRequestFile)) return false;
    if (!SendString(PacketType::FileTransferRequestFile, t_fileName, false));
    return true;
}

bool Client::getString(string& t_string)
{
    int32_t bufferlen;

    if (!getInt32_t(bufferlen)) return false;

    char* buffer = new char[bufferlen + 1];
    buffer[bufferlen] = '\0';

    if (!recvAll(buffer, bufferlen))
    {
        delete[] buffer;
        return false;
    }

    t_string = buffer;
    delete[] buffer;

    return true;
}