#pragma once
#include "header.h"

class HListener
{
public:
	HListener(const shared_ptr<HObject>& pObject, const shared_ptr<function<void(void)>>& pFunc);
	virtual ~HListener() {}

	void SetFunc(const shared_ptr<function<void(void)>>& pFunc) { m_pFunc = pFunc; }
	shared_ptr<function<void(void)>> GetFunc() const { return m_pFunc; }

private:
	shared_ptr<HObject> m_pObject;
	shared_ptr<function<void(void)>> m_pFunc;
};