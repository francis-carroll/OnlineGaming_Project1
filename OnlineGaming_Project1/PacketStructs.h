#pragma once

#include "GameUpdate.h"
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
		ChatMessage(const string& t_message);
		shared_ptr<Packet> toPacket();
	private:
		string m_message;
	};

	class GameUpdate
	{
	public:
		GameUpdate(const UpdateInfo& t_gameData);
		std::shared_ptr<Packet> toPacket();
	private:
		UpdateInfo m_data;
	};
}