#pragma once
#include "header.h"

struct HEventArg
{
	USHORT X, Y;
	union
	{
		USHORT VKey;
		USHORT VMouse;
	};
	LONG LastX, LastY;
	USHORT VWheel;
};

class HListener
{
public:
	HListener(const shared_ptr<HObject>& pObject, const function<void(HEventArg)>& pFunc);
	virtual ~HListener() {}

	void SetFunc(const function<void(HEventArg)>& pFunc);
	function<void(HEventArg)> GetFunc() const;

private:
	shared_ptr<HObject> m_pObject;
	function<void(HEventArg)> m_pFunc;
};