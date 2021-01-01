#pragma once

enum class PacketType
{
	ChatMessage,
	FileTransferRequestFile,
	FileTransfer_EndOfFile,
	FileTransferByteBuffer,
	FileTransferRequestNextBuffer
};