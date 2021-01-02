#pragma once

#include <memory.h>
#include <vector>
#include <string>
#include <iostream>

#include "PacketType.h"
#include "GameUpdate.h"

using namespace std;

class Packet
{
public:
	Packet();
	Packet(const char* t_buffer, const int t_size); //Will use existing allocated buffer and create packet from it
												 //Packet(const Packet & p); //Will allocate new buffer but copy buffer from packet argument
	Packet(PacketType t_p); //Used for when sending a packet that only contains a packet type (Ex. End of File Packet)
	Packet(const shared_ptr<Packet> t_p);

	void append(const shared_ptr<Packet> t_p);
	void append(const PacketType t_p);
	void append(const int32_t t_int32);
	void append(const size_t t_p);
	void append(const Packet& t_p);
	void append(const UpdateInfo& t_p);
	void append(const string& t_str);
	void append(const char* t_buffer, const int t_size); //Will use existing allocated buffer and create packet from it

	vector<int8_t> getBuffer();
private:
	vector<int8_t> m_buffer; //Packet Buffer	
};