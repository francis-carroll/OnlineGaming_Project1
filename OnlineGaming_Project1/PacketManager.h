#pragma once

#include <queue>
#include <mutex>

#include "Packet.h"

using namespace std;

class PacketManager
{
public:
	bool hasPendingPackets();
	void append(shared_ptr<Packet> t_packet);
	shared_ptr<Packet> retrieve();
	void clearPackets();
private:
	queue<shared_ptr<Packet>> m_queuedPackets;
	mutex m_mutexPackets;
};