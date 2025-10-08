#include <iostream>
#include <thread>
#include <chrono>

#include "BLEDevice.h"

typedef unsigned long long ull;
static inline ull GetTime()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now().time_since_epoch()
    ).count();
}

BLEDevice::BLEDevice(const char* mac)
	: mMac(mac)
{
    if (strcmp(mac, ""))
        mTargetMac = true;
}

void BLEDevice::InitializeAdapter()
{
    const std::vector<SimpleBLE::Adapter> adapters = SimpleBLE::Adapter::get_adapters();

    if (adapters.empty())
        throw std::runtime_error("No BLE adapters found!");

    mAdapter = adapters[0];
    mAdapterReady = true;

    mAdapter.set_callback_on_scan_start([]() { std::cout << "Scan started\n"; });
    mAdapter.set_callback_on_scan_stop([&]()
        {
            std::cout << "Scan stopped\n";
            mDeviceStatus = mDeviceStatus == Found ? ReadyToConnect : NotFound;
        });

    mAdapter.set_callback_on_scan_found([&](SimpleBLE::Peripheral peripheral)
        {
            if (!mTargetMac)
                if (peripheral.is_connectable())
                    std::cout << "Found device: " << peripheral.identifier() << " [" << peripheral.address() << "]\n";
            if (!strcmp(peripheral.address().c_str(), mMac))
            {
                std::cout << "Found device: " << peripheral.identifier() << " [" << peripheral.address() << "]\n";
                mPeripheral = peripheral;
                mDeviceStatus = Found;
                mAdapter.scan_stop();
            }
        });
}

bool BLEDevice::Connect(const int msTimeout)
{
    if (!mAdapterReady || IsConnected())
        return false;

    mAdapter.scan_start();
    const ull scanStartTime = GetTime();
    bool hasStopped = false;

    // Wait for scan to find the device or timeout
    while (mDeviceStatus != ReadyToConnect && mDeviceStatus != NotFound)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        if (!hasStopped && (GetTime() - scanStartTime > msTimeout))
        {
            mAdapter.scan_stop();
            hasStopped = true;
        }
    }

    if (mDeviceStatus == NotFound)
    {
        mDeviceStatus = NotConnected;
        return false;
    }

    std::cout << "Connecting...\n";
    mPeripheral.connect();
    mDeviceStatus = Connected;
    std::cout << "Successfully connected to: [" << mPeripheral.address() << "]\n";

    for (auto& s : mPeripheral.services())
    {
        std::cout << "Service: " << s.uuid() << '\n';
        for (auto& c : s.characteristics())
            std::cout << "Chars: " << c.uuid() << '\n';
    }

    return true;
}

void BLEDevice::WriteToUUID(const char* serviceUUID, const char* charUUID, const SimpleBLE::ByteArray data)
{
    if (!IsConnected())
        return;

    mPeripheral.write_command(serviceUUID, charUUID, data);
}

bool BLEDevice::SubscribeToUUID(const char* uuid)
{
    if (!IsConnected())
        return false;

    for (auto& s : mPeripheral.services())
    {
        for (auto& c : s.characteristics())
        {
            if (!strcmp(c.uuid().c_str(), uuid))
            {
                std::cout << "Subscribing to: " << uuid << '\n';
                mPeripheral.notify(s.uuid(), c.uuid(), [&](SimpleBLE::ByteArray bytes)
                    {
                        std::cout << "Received: ";  
                        for (auto b : bytes) std::cout << std::hex << static_cast<int>(b) << ' ';
                        std::cout << '\n';
                    });
                return true;
            }
        }
    }
    return false;
}

void BLEDevice::Disconnect()
{
    if (IsConnected())
        mPeripheral.disconnect();
}

bool BLEDevice::IsConnected() const
{
    return mDeviceStatus == Connected;
}
