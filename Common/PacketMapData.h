#pragma once
#include "Library.h"
#include "Packet.h"
#include "IMap.h"

class COMMON_LIBRARY PacketMapData : public Packet
{
public:
	PacketMapData(int clientId, IMap* map);
	PacketMapData(DataBuffer dataBuffer);
	~PacketMapData();
	DataBuffer GetData();
	
	//TODO Delete m_mapData at destrcutor

	short m_clientId;
	short m_spawnX;
	short m_spawnY;
	short m_mapWidth;
	short m_mapHeight;
	char* m_mapData;
};