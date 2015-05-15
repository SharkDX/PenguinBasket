#pragma once
#include "Library.h"
#include "DataBuffer.h"

#define PROTOCOL_VERSION 0x1234

#define PACKET_HAND_SHAKE			0x01
#define PACKET_MAP_DATA				0x02
#define PACKET_SPAWN_PLAYER			0x03
#define PACKET_PLAYER_POSITION		0x04
#define PACKET_PING					0x05
#define PACKET_PLAYER_CONNECTED		0x06
#define PACKET_PLAYER_DISCONNECTED	0x07
#define PACKET_CHAT_MESSAGE			0x08
#define PACKET_CREATE_BLOCK			0x09
#define PACKET_DESTROY_BLOCK		0x0A

enum DeliveryMethod
{
	UnreliableUnordered,
	ReliableUnordered,
	UnreliableOrdered,
	ReliableOrdered
};

class COMMON_LIBRARY Packet
{
public:
	Packet(unsigned char header, DeliveryMethod deliveryMethod);
	virtual ~Packet();

	virtual DataBuffer GetData() = 0;
	unsigned char GetHeader();
	DeliveryMethod GetDeliveryMethod();

protected:
	unsigned char m_header;
	DeliveryMethod m_deliveryMethod;
};