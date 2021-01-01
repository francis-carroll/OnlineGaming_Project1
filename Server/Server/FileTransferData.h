#pragma once
#include <fstream>

using namespace std;

class FileTransferData
{
public:
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