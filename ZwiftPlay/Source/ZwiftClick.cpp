#include "ZwiftClick.h"

#include <iostream>
#include <Windows.h>

const char* CONTROLS_UUID = "00000002-19ca-4651-86e5-fa29dcdd09d1";
const char* HANDSHAKE_UUID_SERVICE = "00000001-19ca-4651-86e5-fa29dcdd09d1";
const char* HANDSHAKE_UUID = "00000003-19ca-4651-86e5-fa29dcdd09d1";
const char* HANDSHAKE_MSG = "RideOn";

constexpr int CLICK_NOTIFICATION_MESSAGE_TYPE = 0x37;
constexpr int BATTERY_LEVEL_TYPE = 0x19;

constexpr size_t TYPE_INDEX = 0;
constexpr size_t PLUS_INDEX = 2;
constexpr size_t MINUS_INDEX = 4;

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

	std::cout << "[CLICK]: Subscribing to controls\n";
	if (!mDevice.SubscribeToUUID(CONTROLS_UUID, [this](SimpleBLE::ByteArray payload)
		{
			if (payload.size() < 2)
				return;

			if (payload[TYPE_INDEX] == BATTERY_LEVEL_TYPE)
			{
				const int batteryLevel = payload[payload.size() - 1];
				std::cout << "[CLICK]: Battery level: " << batteryLevel << '\n';
				return;
			}

			if (payload[TYPE_INDEX] == CLICK_NOTIFICATION_MESSAGE_TYPE && payload.size() > 4)
			{
				// Handle +
				if (payload[PLUS_INDEX] == 0x00 && !this->buttonStatus[Button::Plus])
				{
					this->PressKey('I');
					this->buttonStatus[Button::Plus] = true;
					std::cout << "[CLICK]: + Pressed\n";
				}
				else if (payload[PLUS_INDEX] != 0x00 && this->buttonStatus[Button::Plus])
				{
					this->buttonStatus[Button::Plus] = false;
					std::cout << "[CLICK]: + Released\n";
				}

				// Handle -
				if (payload[MINUS_INDEX] == 0x00 && !this->buttonStatus[Button::Minus])
				{
					this->PressKey('K');
					this->buttonStatus[Button::Minus] = true;
					std::cout << "[CLICK]: - Pressed\n";
				}
				else if (payload[MINUS_INDEX] != 0x00 && this->buttonStatus[Button::Minus])
				{
					this->buttonStatus[Button::Minus] = false;
					std::cout << "[CLICK]: - Released\n";
				}
				return;
			}
		}))
		throw std::runtime_error("[CLICK]: Could not subscribe to controls");

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
	static_assert(false, "Implement PressKey");
}
#endif
