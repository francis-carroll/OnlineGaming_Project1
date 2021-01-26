#include "PacketStructs.h"

PS::ChatMessage::ChatMessage(const string& t_message) :
	m_message(t_message)
{
}

std::shared_ptr<Packet> PS::ChatMessage::toPacket()
{
	std::shared_ptr<Packet> p = std::make_shared<Packet>();
	p->append(PacketType::ChatMessage);
	p->append(m_message.size());
	p->append(m_message);
	return p;
}

PS::GameUpdate::GameUpdate(const UpdateInfo& t_gameData)
{
	m_data = t_gameData;
}

std::shared_ptr<Packet> PS::GameUpdate::toPacket(PacketType t_packetType)
{
	std::shared_ptr<Packet> p = std::make_shared<Packet>();
	p->append(t_packetType);
	p->append(sizeof(m_data));
	p->append(m_data);
	return p;
}

PS::StateUpdate::StateUpdate(const StateInfo& t_gameState)
{
	m_data = t_gameState;
}

std::shared_ptr<Packet> PS::StateUpdate::toPacket(PacketType t_packetType)
{
	std::shared_ptr<Packet> p = std::make_shared<Packet>();
	p->append(t_packetType);
	p->append(sizeof(m_data));
	p->append(m_data);
	return p;
}

PS::StartUpdate::StartUpdate(const StartInfo& t_gameData)
{
	m_data = t_gameData;
}

std::shared_ptr<Packet> PS::StartUpdate::toPacket(PacketType t_packetType)
{
	std::shared_ptr<Packet> p = std::make_shared<Packet>();
	p->append(t_packetType);
	p->append(sizeof(m_data));
	p->append(m_data);
	return p;
}

PS::EndUpdate::EndUpdate(const EndInfo& t_gameState)
{
	m_data = t_gameState;
}

std::shared_ptr<Packet> PS::EndUpdate::toPacket(PacketType t_packetType)
{
	std::shared_ptr<Packet> p = std::make_shared<Packet>();
	p->append(t_packetType);
	p->append(sizeof(m_data));
	p->append(m_data);
	return p;
}
