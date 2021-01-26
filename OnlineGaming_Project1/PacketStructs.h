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

	class StartUpdate
	{
	public:
		StartUpdate(const StartInfo& t_gameData);
		std::shared_ptr<Packet> toPacket(PacketType t_packetType);
	private:
		StartInfo m_data;
	};

	class GameUpdate
	{
	public:
		GameUpdate(const UpdateInfo& t_gameData);
		std::shared_ptr<Packet> toPacket(PacketType t_packetType);
	private:
		UpdateInfo m_data;
	};

	class StateUpdate
	{
	public:
		StateUpdate(const StateInfo& t_gameState);
		std::shared_ptr<Packet> toPacket(PacketType t_packetType);
	private:
		StateInfo m_data;
	};

	class EndUpdate
	{
	public:
		EndUpdate(const EndInfo& t_gameState);
		std::shared_ptr<Packet> toPacket(PacketType t_packetType);
	private:
		EndInfo m_data;
	};
}