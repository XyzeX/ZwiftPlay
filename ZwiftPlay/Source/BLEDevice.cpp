#include <iostream>
#include <thread>
#include <chrono>

#include "BLEDevice.h"

BLEDevice::BLEDevice(const char* mac)
	: mMac(mac)
{
    if (strcmp(mac, ""))
        mTargetMac = true;
}

bool BLEDevice::InitializeAdapter()
{
    const std::vector<SimpleBLE::Adapter> adapters = SimpleBLE::Adapter::get_adapters();

    if (adapters.empty())
        throw std::runtime_error("No BLE adapters found!");

    mAdapter = adapters[0];
    mAdapterReady = true;


    mAdapter.set_callback_on_scan_start([]() { std::cout << "Scan started\n"; });
    mAdapter.set_callback_on_scan_stop([&]() { std::cout << "Scan stopped\n"; mDeviceStatus = DeviceStatus::Connecting; });


    mAdapter.set_callback_on_scan_found([&](SimpleBLE::Peripheral peripheral)
        {
            if (!mTargetMac)
                if (peripheral.is_connectable())
                    std::cout << "Found device: " << peripheral.identifier() << " [" << peripheral.address() << "]\n";
            if (!strcmp(peripheral.address().c_str(), mMac))
            {
                std::cout << "Found device: " << peripheral.identifier() << " [" << peripheral.address() << "]\n";
                mPeripheral = peripheral;
                mDeviceStatus = DeviceStatus::Found;
                mAdapter.scan_stop();
            }
        });
    
	return true;
}

bool BLEDevice::Connect(const int scanTimeout)
{
    if (!mAdapterReady)
        return false;

    // START SCAN
    mAdapter.scan_start();
    while (mDeviceStatus != DeviceStatus::Connecting)
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

    std::cout << "Connecting...\n";

    try
    {
        mPeripheral.connect();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Caught exception: " << e.what() << '\n';
    }
    
    std::cout << "Successfully connected to: [" << mPeripheral.address() << "]\n";

    return true;
}
