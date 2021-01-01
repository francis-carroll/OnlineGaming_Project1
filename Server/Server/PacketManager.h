#pragma once

#include <queue>
#include <mutex>

#include "Packet.h"

using namespace std;

class PacketManager
{
public:
	bool hasPendingPackets();
	void append(Packet t_packet);
	Packet retrieve();
	void clearPackets();
private:
	queue<Packet> m_queuedPackets;
	mutex m_mutexPackets;
};

