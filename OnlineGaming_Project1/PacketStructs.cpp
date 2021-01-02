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

std::shared_ptr<Packet> PS::GameUpdate::toPacket()
{
	std::shared_ptr<Packet> p = std::make_shared<Packet>();
	p->append(PacketType::Update);
	p->append(sizeof(m_data));
	p->append(m_data);
	return p;
}