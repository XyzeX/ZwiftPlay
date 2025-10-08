
class ZwiftDevice
{
public:

	//virtual void SetKeyMap(void* _);
	virtual void Initialize() = 0;
	virtual bool Connect() = 0;
	virtual void Close() = 0;

	virtual bool IsConnected() const = 0;
};
