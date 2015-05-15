#include "PacketMapData.h"

PacketMapData::PacketMapData(int clientId, IMap* map) :
	Packet(PACKET_MAP_DATA, DeliveryMethod::ReliableOrdered)
{
	m_clientId = clientId;
	m_spawnX = map->GetSpawnX();
	m_spawnY = map->GetSpawnY();
	m_mapWidth = map->GetWidth();
	m_mapHeight = map->GetHeight();
	m_mapData = new char[m_mapWidth * m_mapHeight];

	for (int i = 0; i < m_mapWidth; i++)
	{
		for (int j = 0; j < m_mapHeight; j++)
			m_mapData[j * m_mapWidth + i] = map->GetBlock(i, j);
	}
}

PacketMapData::PacketMapData(DataBuffer dataBuffer) :
	Packet(PACKET_MAP_DATA, DeliveryMethod::ReliableOrdered)
{
	m_clientId = dataBuffer.ReadShort();
	m_spawnX = dataBuffer.ReadShort();
	m_spawnY = dataBuffer.ReadShort();

	m_mapWidth = dataBuffer.ReadShort();
	m_mapHeight = dataBuffer.ReadShort();

	m_mapData = new char[m_mapWidth * m_mapHeight];

	for (int i = 0; i < m_mapWidth; i++)
	{
		for (int j = 0; j < m_mapHeight; j++)
			m_mapData[j * m_mapWidth + i] = dataBuffer.ReadChar();
	}
}

PacketMapData::~PacketMapData()
{
	delete[] m_mapData;
}

DataBuffer PacketMapData::GetData()
{
	DataBuffer data;
	data.WriteChar(m_header);
	data.WriteShort(m_clientId);
	data.WriteShort(m_spawnX);
	data.WriteShort(m_spawnY);
	data.WriteShort(m_mapWidth);
	data.WriteShort(m_mapHeight);

	for (int i = 0; i < m_mapWidth; i++)
	{
		for (int j = 0; j < m_mapHeight; j++)
			data.WriteChar(m_mapData[j * m_mapWidth + i]);
	}

	return data;
}