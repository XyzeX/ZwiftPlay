#include <atomic>

#include <simpleble/SimpleBLE.h>

enum DeviceStatus
{
	NotConnected,
	Found,
	Connecting,
};

class BLEDevice
{
public:
	BLEDevice(const char* mac = "");

	bool InitializeAdapter();
	bool Connect(const int scanTimeout = 50000);

private:
	SimpleBLE::Adapter mAdapter;
	SimpleBLE::Peripheral mPeripheral;
	const char* mMac;

	bool mTargetMac = false;
	bool mAdapterReady = false;
	std::atomic<DeviceStatus> mDeviceStatus = DeviceStatus::NotConnected;
};
