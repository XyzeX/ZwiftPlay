#include "ZwiftClick.h"

#include <iostream>
#include <Windows.h>

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
	PressKey('A');
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

#ifdef _WINDOWS
void ZwiftClick::PressKey(const char vkCode) const
{
	INPUT input{ 0 };
	input.type = INPUT_KEYBOARD;
	input.ki.wVk = vkCode;

	SendInput(1, &input, sizeof(INPUT));

	input.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &input, sizeof(INPUT));
}
#else
void ZwiftClick::PressKey(const char vkCode) const
{
	std::cerr << "No implementation yet...\n";
	static_assert("Implement PressKey");
}
#endif
