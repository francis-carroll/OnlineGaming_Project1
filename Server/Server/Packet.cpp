#include "Packet.h"

Packet::Packet()
{
	m_buffer = nullptr;
	m_size = 0;
}

Packet::Packet(char* t_buffer, int t_size)
{
	m_buffer = t_buffer;
	m_size = t_size;
}

Packet::Packet(const Packet& t_packet)
{
	m_size = t_packet.m_size;
	m_buffer = new char[m_size];
	memcpy(m_buffer, t_packet.m_buffer, m_size);
}

char* Packet::getBuffer()
{
	return m_buffer;
}

int Packet::getSize()
{
	return m_size;
}
