#include "Packet.h"


Packet::Packet(unsigned char header, DeliveryMethod deliveryMethod)
{
	m_header = header;
	m_deliveryMethod = deliveryMethod;
}

Packet::~Packet()
{

}

unsigned char Packet::GetHeader()
{
	return m_header;
}

DeliveryMethod Packet::GetDeliveryMethod()
{
	return m_deliveryMethod;
}