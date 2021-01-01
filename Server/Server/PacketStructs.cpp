#include "PacketStructs.h"

PS::ChatMessage::ChatMessage(string t_messgae)
{
	m_message = t_messgae;
}

Packet PS::ChatMessage::toPacket()
{
	const int packetSize = sizeof(int32_t) * 2 + m_message.size() * sizeof(char);
	char* buffer = new char[packetSize];

	int32_t packetType = htonl((int32_t)PacketType::ChatMessage);
	int32_t messageSize = htonl(m_message.size());
	memcpy(buffer, &packetType, sizeof(int32_t));
	memcpy(buffer + sizeof(int32_t), &messageSize, sizeof(int32_t));
	memcpy(buffer + sizeof(int32_t) * 2, m_message.c_str(), m_message.size() * sizeof(char));
	Packet p(buffer, packetSize);
	return p;
}
