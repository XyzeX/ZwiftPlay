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

	enum Button { Plus = 0, Minus = 1 };
	bool buttonStatus[2]{ false, false };
};
