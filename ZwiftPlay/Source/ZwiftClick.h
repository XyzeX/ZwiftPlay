#include "ZwiftDevice.h"
#include "BLEDevice.h"

class ZwiftClick : ZwiftDevice
{
public:
	ZwiftClick(const char* mac);

	void Initialize() override;
	bool Connect() override;
	void Close() override;

	bool IsConnected() const;

private:
	void PressKey(const char vkCode) const;

private:
	BLEDevice mDevice;
};
