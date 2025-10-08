#include "ZwiftClick.h"

#include <iostream>
#include <thread>
#include <chrono>

int main()
{
#ifdef _WINDOWS
	std::cout << "Make sure the Zwift Click/Play is NOT paired in windows\n";
#endif

	ZwiftClick click = ZwiftClick("ce:e3:bd:7f:c2:9c");
	click.Initialize();

	while (!click.Connect())
		std::cout << "Connection failed, retrying\n";

	while (true)
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	click.Close();
}
