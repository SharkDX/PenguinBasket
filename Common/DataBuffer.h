#pragma once
#include "Library.h"

#include <vector>

class COMMON_LIBRARY DataBuffer
{
public:
	DataBuffer();
	DataBuffer(char* buffer, int size);
	~DataBuffer();

	char *GetRawData() { return &m_data[0]; };
	int GetDataSize() { return m_data.size(); };

	void WriteChar(char value);
	void WriteShort(short value);
	void WriteInt(int value);
	void WriteLong(long value);
	void WriteFloat(float value);
	void WriteString(std::string value);

	char ReadChar();
	short ReadShort();
	int ReadInt();
	long ReadLong();
	float ReadFloat();
	std::string ReadString();

private:
	std::vector<char> m_data;
	unsigned int m_position;
};