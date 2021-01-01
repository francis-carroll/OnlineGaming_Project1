#pragma once

#include <memory.h>

using namespace std;

class Packet
{
public:
	Packet();
	Packet(char* t_buffer, int t_size);
	Packet(const Packet& t_packet);

	char* getBuffer();
	int getSize();
private:
	int m_size;
	char* m_buffer;
};

