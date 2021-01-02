#pragma once

#include "PacketManager.h"

class Connection
{
public:
	Connection() {}
	Connection(int t_id, SOCKET t_socket)
	{
		socket = t_socket;
		id = t_id;
		activeConnection = true;
	}

	int id;
	bool activeConnection;
	SOCKET socket;
	PacketManager pm;
};