#include "Packet.h"

#include <cstdint>
#include <WinSock2.h> 

Packet::Packet()
{
}

Packet::Packet(const char* t_buffer,const int t_size)
{
	append(t_buffer, t_size);
}

Packet::Packet(PacketType p)
{
	append(p);
}

Packet::Packet(const shared_ptr<Packet> p)
{
	append(p);
}

void Packet::append(const shared_ptr<Packet> p)
{
	append((const char*)&(p->m_buffer[0]), p->m_buffer.size());
}

void Packet::append(const PacketType p)
{
	append((std::int32_t)p);
}

void Packet::append(const int32_t int32)
{
	int32_t val = htonl((int32_t)int32);
	append((const char*)&val, sizeof(int32_t));
}

void Packet::append(const size_t p)
{
	append((std::int32_t)p);
}

void Packet::append(const Packet& p)
{
	append((const char*)&p.m_buffer, p.m_buffer.size());
}

void Packet::append(const UpdateInfo& t_p)
{
	append((const char*)&t_p, sizeof(UpdateInfo));
}

void Packet::append(const StateInfo& t_p)
{
	append((const char*)&t_p, sizeof(StateInfo));
}

void Packet::append(const StartInfo& t_p)
{
	append((const char*)&t_p, sizeof(StartInfo));
}

void Packet::append(const EndInfo& t_p)
{
	append((const char*)&t_p, sizeof(EndInfo));
}

void Packet::append(const string& str)
{
	append(str.c_str(), str.size());
}

void Packet::append(const char* buffer, const int size)
{
	m_buffer.insert(m_buffer.end(), buffer, buffer + size);
}

vector<int8_t> Packet::getBuffer()
{
	return m_buffer;
}