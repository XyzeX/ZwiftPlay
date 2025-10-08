#include "ZwiftClick.h"

#include <iostream>

const char* CONTROLS_UUID = "00000002-19ca-4651-86e5-fa29dcdd09d1";
const char* BATTERY_UUID = "00002a19-0000-1000-8000-00805f9b34fb";
const char* HANDSHAKE_UUID_SERVICE = "00000001-19ca-4651-86e5-fa29dcdd09d1";
const char* HANDSHAKE_UUID = "00000003-19ca-4651-86e5-fa29dcdd09d1";
const char* HANDSHAKE_MSG = "RideOn";

ZwiftClick::ZwiftClick(const char* mac)
	: mDevice(mac)
{
	
}

void ZwiftClick::Initialize()
{
	mDevice.InitializeAdapter();
}

bool ZwiftClick::Connect()
{
	if (!mDevice.Connect(5000))
		return false;

	std::cout << "Subscribing to controls\n";
	if (!mDevice.SubscribeToUUID(CONTROLS_UUID))
		throw "Could not subscribe to controls\n";

	std::cout << "Subscribing to battery indicator\n";
	if (!mDevice.SubscribeToUUID(BATTERY_UUID))
		std::cerr << "Could Not subscribe to battery indicator\n";

	mDevice.WriteToUUID(HANDSHAKE_UUID_SERVICE, HANDSHAKE_UUID, HANDSHAKE_MSG);
	return true;
}

void ZwiftClick::Close()
{
	mDevice.Disconnect();
}

bool ZwiftClick::IsConnected() const
{
	return mDevice.IsConnected();
}
