#pragma once
#include "Library.h"
#include "Packet.h"

class COMMON_LIBRARY PacketPing : public Packet
{
public:
	PacketPing();
	DataBuffer GetData();
};