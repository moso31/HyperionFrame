#pragma once
#include "header.h"

class HListener
{
public:
	HListener(const shared_ptr<HObject>& pObject, const function<void(void)>& pFunc);
	virtual ~HListener() {}

	void SetFunc(const function<void(void)>& pFunc);
	function<void(void)> GetFunc() const;

private:
	shared_ptr<HObject> m_pObject;
	function<void(void)> m_pFunc;
};