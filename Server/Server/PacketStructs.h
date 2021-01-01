#pragma once

#pragma comment(lib,"ws2_32.lib")

#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <string>

#include "PacketType.h"
#include "Packet.h"

using namespace std;

namespace PS
{
	class ChatMessage
	{
	public:
		ChatMessage(string t_message);
		Packet toPacket();
	private:
		string m_message;
	};
}