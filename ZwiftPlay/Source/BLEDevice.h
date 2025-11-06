#include <iostream>
#include <atomic>

#include <simpleble/SimpleBLE.h>

enum DeviceStatus
{
	NotConnected,
	NotFound,
	Found,
	ReadyToConnect,
	Connected
};

class BLEDevice
{
public:
	BLEDevice(const char* mac = "");

	void InitializeAdapter();
	bool Connect(const int msTimeout);

	void WriteToUUID(const char* serviceUUID, const char* charUUID, SimpleBLE::ByteArray data);

	template<typename Callback>
	bool SubscribeToUUID(const char* uuid, Callback&& callback)
	{
		if (!IsConnected())
			return false;

		for (auto& s : mPeripheral.services())
		{
			for (auto& c : s.characteristics())
			{
				if (!std::strcmp(c.uuid().c_str(), uuid))
				{
					mPeripheral.notify(s.uuid(), c.uuid(), std::forward<Callback>(callback));
					return true;
				}
			}
		}
		return false;
	}
	void Disconnect();

	bool IsConnected() const;

private:
	SimpleBLE::Adapter mAdapter;
	SimpleBLE::Peripheral mPeripheral;
	const char* mMac;

	bool mTargetMac = false;
	bool mAdapterReady = false;
	std::atomic<DeviceStatus> mDeviceStatus = DeviceStatus::NotConnected;
};
