#include "DataBuffer.h"

DataBuffer::DataBuffer()
{

}

DataBuffer::DataBuffer(char* buffer, int size)
{
	for (int i = 0; i < size; i++)
		m_data.push_back(buffer[i]);

	m_position = 0;
}

DataBuffer::~DataBuffer()
{

}

void DataBuffer::WriteChar(char value)
{
	m_data.push_back(value);
}

void DataBuffer::WriteShort(short value)
{
	m_data.push_back(((char*)&value)[0]);
	m_data.push_back(((char*)&value)[1]);
}

void DataBuffer::WriteInt(int value)
{
	m_data.push_back(((char*)&value)[0]);
	m_data.push_back(((char*)&value)[1]);
	m_data.push_back(((char*)&value)[2]);
	m_data.push_back(((char*)&value)[3]);
}

void DataBuffer::WriteLong(long value)
{
	m_data.push_back(((char*)&value)[0]);
	m_data.push_back(((char*)&value)[1]);
	m_data.push_back(((char*)&value)[2]);
	m_data.push_back(((char*)&value)[3]);
	m_data.push_back(((char*)&value)[4]);
	m_data.push_back(((char*)&value)[5]);
	m_data.push_back(((char*)&value)[6]);
	m_data.push_back(((char*)&value)[7]);
}

void DataBuffer::WriteFloat(float value)
{
	m_data.push_back(((char*)&value)[0]);
	m_data.push_back(((char*)&value)[1]);
	m_data.push_back(((char*)&value)[2]);
	m_data.push_back(((char*)&value)[3]);
}

void DataBuffer::WriteString(std::string value)
{
	unsigned short length = value.length();
	WriteShort(length);

	for (int i = 0; i < length; i++)
		m_data.push_back(value[i]);
}

char DataBuffer::ReadChar()
{
	char value = m_data[m_position];
	m_position += sizeof(char);
	return value;
}

short DataBuffer::ReadShort()
{
	short value = *(short*)&m_data[m_position];
	m_position += sizeof(short);
	return value;
}

int DataBuffer::ReadInt()
{
	int value = *(int*)&m_data[m_position];
	m_position += sizeof(int);
	return value;
}

long DataBuffer::ReadLong()
{
	long value = *(long*)&m_data[m_position];
	m_position += sizeof(long);
	return value;
}

float DataBuffer::ReadFloat()
{
	float value = *(float*)&m_data[m_position];
	m_position += sizeof(float);
	return value;
}

std::string DataBuffer::ReadString()
{
	std::string value;
	short length = ReadShort();
	
	for (int i = 0; i < length; i++)
		value += m_data[m_position + i];

	m_position += length;
	return value;
}