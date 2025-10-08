#include <iostream>

#include "BLEDevice.h"

int main()
{
#ifdef _WINDOWS
	std::cout << "Make sure the Zwift Click/Play is NOT paired in windows\n";
#endif

	BLEDevice device = BLEDevice("ce:e3:bd:7f:c2:9c");
	//BLEDevice device = BLEDevice("88:c9:e8:6a:7a:43");
	//BLEDevice device = BLEDevice("44:0c:de:db:9e:ee");

	if (!device.InitializeAdapter())
		throw "Bluetooth module could not be found\n";

	while (!device.Connect(5000))
		std::cout << "Connection failed, retrying\n";
}
