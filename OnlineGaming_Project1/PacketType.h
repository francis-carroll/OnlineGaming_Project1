#pragma once

enum class PacketType
{
	SetupClient,
	UpdateRecv,
	ChatMessage,
	UpdateState,
	RecieveState,
	GameOver,
	SetupVisuals
};