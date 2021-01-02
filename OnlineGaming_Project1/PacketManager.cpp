#include "PacketManager.h"

bool PacketManager::hasPendingPackets()
{
    lock_guard<mutex> lock(m_mutexPackets);
    return (!m_queuedPackets.empty());
}

void PacketManager::append(shared_ptr<Packet> t_packet)
{
    lock_guard<mutex> lock(m_mutexPackets); //lock mutex so two threads dont access data at same time
    m_queuedPackets.push(std::move(t_packet));
}

shared_ptr<Packet> PacketManager::retrieve()
{
    lock_guard<mutex> lock(m_mutexPackets);
    shared_ptr<Packet> p = m_queuedPackets.front();
    m_queuedPackets.pop();
    return p;
}

void PacketManager::clearPackets()
{
    lock_guard<mutex> lock(m_mutexPackets);
    m_queuedPackets = queue<shared_ptr<Packet>>{};
}
