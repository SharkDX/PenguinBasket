#include "Client.h"

Client::Client()
{
	m_ENetClient = enet_host_create(NULL /* create a client host */,
		1 /* only allow 1 outgoing connection */,
		2 /* allow up 2 channels to be used, 0 and 1 */,
		57600 / 8 /* 56K modem with 56 Kbps downstream bandwidth */,
		14400 / 8 /* 56K modem with 14 Kbps upstream bandwidth */);

}

Client::~Client()
{
	enet_host_destroy(m_ENetClient);
}

bool Client::Connect(std::string host, int port)
{
	ENetAddress address;
	ENetEvent event;
	/* Connect to the given hostname and port */
	enet_address_set_host(&address, host.c_str());
	address.port = port;
	/* Initiate the connection, allocating the three channels 0, 1 and 2. */
	m_ENetServer = enet_host_connect(m_ENetClient, &address, 3, 0);
	if (m_ENetServer == NULL)
	{
		fprintf(stderr,
			"No available peers for initiating an ENet connection.\n");
		exit(EXIT_FAILURE);
	}

	/* Wait up to 5 seconds for the connection attempt to succeed. */
	if (enet_host_service(m_ENetClient, &event, 5000) > 0 &&
		event.type == ENET_EVENT_TYPE_CONNECT)
	{
		std::cout << "Connection to " << host << ":" << port << " has succeed" << std::endl;
		return true;
	}
	else
	{
		/* Either the 5 seconds are up or a disconnect event was */
		/* received. Reset the peer in the event the 5 seconds   */
		/* had run out without any significant event.            */
		enet_peer_reset(m_ENetServer);
		std::cout << "Connection to " << host << ":" << port << " failed" << std::endl;
		return false;
	}
}

void Client::Disconnect()
{
	enet_peer_disconnect_now(m_ENetServer, this->clientId);
}

void Client::Start()
{
	m_RunThread = std::thread(&Client::Run, this);
	m_SenderThread = std::thread(&Client::Sender, this);
}

void Client::Run()
{
	m_IsRunning = true;
	while (m_IsRunning)
	{
		ENetEvent event;
		/* Wait up to 1000 milliseconds for an event. */
		while (enet_host_service(m_ENetClient, &event, 1000) > 0)
		{
			switch (event.type)
			{
			case ENET_EVENT_TYPE_RECEIVE:
				/*printf("A packet of length %u containing %s was received from %s on channel %u.\n",
				event.packet->dataLength,
				event.packet->data,
				event.peer->data,
				event.channelID);*/

				if (processPacketFunc != nullptr) {
					processPacketFunc(event.packet, event.peer);
				}

				/* Clean up the packet now that we're done using it. */
				enet_packet_destroy(event.packet);
				break;
			case ENET_EVENT_TYPE_DISCONNECT:
				printf("%s disconnected.\n", event.peer->data);
				/* Reset the peer's client information. */
				event.peer->data = NULL;
			}
		}
	}
}

void Client::SendData(void* data, int size, int channel, int flags)
{
	ENetPacket* packet = enet_packet_create(data, size, flags);
	enet_peer_send(m_ENetServer, channel, packet);
}

void Client::SendPacket(std::shared_ptr<Packet> packet)
{
	m_OutgoingPacketsMutex.lock();
	m_OutgoingPackets.push(packet);
	m_OutgoingPacketsMutex.unlock();
}

void Client::Sender()
{
	while (m_IsRunning)
	{
		m_OutgoingPacketsMutex.lock();

		if (m_OutgoingPackets.size() < 1)
		{
			m_OutgoingPacketsMutex.unlock();
			std::this_thread::sleep_for(std::chrono::milliseconds(20)); //TODO Figure out the most optimized sleep time for this task
			continue;
		}

		std::shared_ptr<Packet> p = m_OutgoingPackets.front();
		m_OutgoingPackets.pop();
		m_OutgoingPacketsMutex.unlock();

		int channel = 0;
		int flags = 0;

		switch (p->GetDeliveryMethod())
		{
		case DeliveryMethod::ReliableOrdered:
			channel = 0;
			flags = ENET_PACKET_FLAG_RELIABLE;
			break;
		case DeliveryMethod::UnreliableOrdered:
			channel = 1;
			flags = 0;
			break;
		case DeliveryMethod::UnreliableUnordered:
			channel = 2;
			flags = ENET_PACKET_FLAG_UNSEQUENCED;
			break;
		default:
			assert(p->GetDeliveryMethod() != DeliveryMethod::ReliableUnordered); //TODO Check how to use this behavoiur with ENet
			break;

		}

		DataBuffer data = p->GetData();
		SendData(data.GetRawData(), data.GetDataSize(), channel, flags);

		enet_host_flush(m_ENetClient); //Temp?
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		/*
		if (p->GetHeader() == PACKET_PLAYER_POSITION)
		{
			std::shared_ptr<PacketPlayerPosition> p2 = std::static_pointer_cast<PacketPlayerPosition>(p);
			std::cout << p2->m_PlayerID << "," << p2->m_positionX << "," << p2->m_positionY << std::endl;
		}
		*/
			
	}
}