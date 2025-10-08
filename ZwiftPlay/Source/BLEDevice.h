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
	bool SubscribeToUUID(const char* uuid);
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
