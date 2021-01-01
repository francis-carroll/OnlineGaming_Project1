#include "PacketManager.h"

bool PacketManager::hasPendingPackets()
{
    return (m_queuedPackets.size() > 0);
}

void PacketManager::append(Packet t_packet)
{
    lock_guard<mutex> lock(m_mutexPackets);
    m_queuedPackets.push(t_packet);
}

Packet PacketManager::retrieve()
{
    lock_guard<mutex> lock(m_mutexPackets);
    Packet p = m_queuedPackets.front();
    m_queuedPackets.pop();
    return p;
}

void PacketManager::clearPackets()
{
    lock_guard<mutex> lock(m_mutexPackets);
    queue<Packet> empty;
    swap(m_queuedPackets, empty);
}
