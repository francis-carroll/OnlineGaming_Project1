#pragma once

enum class PacketType
{
	Update,
	ChatMessage,
	FileTransferRequestFile,
	FileTransfer_EndOfFile,
	FileTransferByteBuffer,
	FileTransferRequestNextBuffer
};