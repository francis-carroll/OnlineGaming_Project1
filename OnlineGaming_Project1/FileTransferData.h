#pragma once
#include <fstream>

using namespace std;

struct FileTransferData
{
	static const int bufferSize = 8192;
	string fileName;
	int bytesWritten;
	streampos fileOffset;
	int fileSize;
	int remainingBytes;
	ifstream inFileStream;
	ofstream outFileStream;
	char buffer[bufferSize];
};