#include "Client.h"
#include <Game.h>
#include "Player.h"
#include "BasePlayer.h"

Client::Client(string t_ip, int t_port) : 
    m_connection(make_shared<Connection>())
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
}

Client::~Client()
{
    m_terminateThreads = true;
    for (std::thread* t : m_threads) //Wait for all created threads to end...
    {
        t->join();
    }
}

bool Client::connectSocket(Game* t_game)
{
    m_connection->socket = socket(AF_INET, SOCK_STREAM, NULL);
    if (connect(m_connection->socket, (SOCKADDR*)&m_address, m_addressLenght) != 0)
    {
        MessageBoxA(NULL, "Failed to connect", "Error", MB_OK | MB_ICONERROR);
        return false;
    }

    std::thread CT(clientThread, std::ref(*this), t_game);
    CT.detach();
    m_threads.push_back(&CT);

    std::thread PST(packetSenderThread, std::ref(*this));
    PST.detach();
    m_threads.push_back(&PST);

    return true;
}

bool Client::closeConnection()
{
    if (closesocket(m_connection->socket) == SOCKET_ERROR)
    {
        if (WSAGetLastError() == WSAENOTSOCK) return true;

        string errorMessage = "Failed to close the socket. Winsock error: " + to_string(WSAGetLastError()) + ".";
        MessageBoxA(NULL, errorMessage.c_str(), "Error", MB_OK | MB_ICONERROR);

        return false;
    }
    return true;
}

bool Client::processPacket(PacketType t_packetType, Game* t_game)
{
    switch (t_packetType)
    {
    case PacketType::UpdateRecv:
    {
        UpdateInfo temp;
        getUpdateInfo(temp);

        t_game->getPlayers()->at(temp.t_id)->setPosition(temp.pos);
        t_game->getPlayers()->at(temp.t_id)->setColor(temp.playerType);

        break;
    }
    case PacketType::SetupClient:
    {
        StartInfo temp;
        getStartInfo(temp);

        if (t_game->getContainer() != nullptr)
        {
            t_game->getPlayers()->clear();
            for (int i = 0; i < 3; i++)
            {
                t_game->getPlayers()->push_back(new Player(i));
                t_game->getPlayers()->at(i)->setPosition(Vector2f(-100.0f, -100.0f));
            }

            t_game->getPlayers()->at(temp.t_id) = t_game->getContainer();
            t_game->getPlayers()->at(temp.t_id)->setID(temp.t_id);
        }

        t_game->getPlayers()->at(temp.t_id)->setColor(temp.playerType);
        t_game->getPlayers()->at(temp.t_id)->setColorPlayer(temp.playerType);
        t_game->getPlayers()->at(temp.t_id)->setTarget(temp.target);
        t_game->getPlayers()->at(temp.t_id)->setPosition(temp.pos);

        UpdateInfo updateData;
        updateData.t_id = t_game->getPlayers()->at(temp.t_id)->getID();
        updateData.pos = t_game->getPlayers()->at(temp.t_id)->getPosition();
        updateData.playerType = t_game->getPlayers()->at(temp.t_id)->getColorPlayer();

        PS::GameUpdate is(updateData);
        m_connection->pm.append(is.toPacket(PacketType::UpdateRecv));
        
        break;
    }
    case PacketType::GameOver:
    {
        EndInfo state;
        getEndInfo(state);

        t_game->m_target = state.target;
        t_game->m_winner = state.winner;

        break;
    }
    case PacketType::RecieveState:
    {
        StateInfo state;
        getStateInfo(state);

        t_game->getContainer()->setState(state.m_gameState);
        break;
    }
    case PacketType::ChatMessage:
    {
        string message;
        if (!getString(message)) return false;
        cout << message << endl;
        break;
    }
    default:
        cout << "Unreconised Packet Type" << endl;
        break;
    }
    return true;
}

void Client::clientThread(Client& t_client, Game* t_game)
{
    PacketType packetType;
    while (true)
    {
        if (t_client.m_terminateThreads) break;

        if (!t_client.getPacketType(packetType)) break;

        if (!t_client.processPacket(packetType, t_game)) break;
    }
    cout << "Lost connection to the server" << endl;

    t_client.disconnectClient(t_client.m_connection);
}

void Client::packetSenderThread(Client& t_client)
{
    while (true)
    {
        if (t_client.m_terminateThreads == true)
            break;
        if (t_client.m_connection->pm.hasPendingPackets()) //If there are pending packets for this connection's packet manager
        {
            std::shared_ptr<Packet> p = t_client.m_connection->pm.retrieve(); //Retrieve packet from packet manager
            if (!t_client.sendAll((char*)(&p->getBuffer()[0]), p->getBuffer().size())) //send packet to connection
            {
                std::cout << "Failed to send packet to ID: " << t_client.m_connection->id << std::endl; //Print out if failed to send packet
            }
        }
        Sleep(5);
    }
    std::cout << "Ending Packet Sender Thread..." << std::endl;
}

void Client::disconnectClient(shared_ptr<Connection> t_connection)
{
    lock_guard<shared_mutex> lock(m_connectionManagerMutex);

    t_connection->pm.clearPackets();
    closesocket(t_connection->socket);
    std::cout << "Closed client: " << t_connection->id << "." << std::endl;
}

bool Client::sendAll(char* t_data, int t_totalBytes)
{
    int bytesSent = 0;
    while (bytesSent < t_totalBytes)
    {
        int returnCheck = send(m_connection->socket, t_data + bytesSent, t_totalBytes - bytesSent, NULL);
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
        int returnCheck = recv(m_connection->socket, t_data + bytesRecieved, t_totalBytes - bytesRecieved, NULL);
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

void Client::sendUpdateInfo(UpdateInfo t_gameData)
{
    PS::GameUpdate update(t_gameData);
    m_connection->pm.append(update.toPacket(PacketType::UpdateRecv));
}

bool Client::getPacketType(PacketType& t_packetType)
{
    int packetType;
    if (!getInt32_t(packetType)) return false;
    t_packetType = (PacketType)packetType;
    return true;
}

void Client::sendString(string t_string)
{
    PS::ChatMessage message(t_string);
    m_connection->pm.append(message.toPacket());
}

shared_ptr<Connection> Client::getClientConnection()
{
    return m_connection;
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

bool Client::getUpdateInfo(UpdateInfo& t_gameData)
{
    int32_t bufferlen;

    if (!getInt32_t(bufferlen)) return false;

    if (bufferlen == 0) return 0;

    return recvAll((char*)&t_gameData, bufferlen);
}

bool Client::getStateInfo(StateInfo& t_data)
{
    int32_t bufferlen;

    if (!getInt32_t(bufferlen)) return false;

    if (bufferlen == 0) return 0;

    return recvAll((char*)&t_data, bufferlen);
}

bool Client::getStartInfo(StartInfo& t_data)
{
    int32_t bufferlen;

    if (!getInt32_t(bufferlen)) return false;

    if (bufferlen == 0) return 0;

    return recvAll((char*)&t_data, bufferlen);
}

bool Client::getEndInfo(EndInfo& t_data)
{
    int32_t bufferlen;

    if (!getInt32_t(bufferlen)) return false;

    if (bufferlen == 0) return 0;

    return recvAll((char*)&t_data, bufferlen);
}
