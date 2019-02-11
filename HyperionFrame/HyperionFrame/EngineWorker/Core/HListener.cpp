#include "HListener.h"

HListener::HListener(const shared_ptr<HObject>& pObject, const function<void(void)>& pFunc) :
	m_pObject(pObject),
	m_pFunc(pFunc)
{
}

void HListener::SetFunc(const function<void(void)>& pFunc)
{
	m_pFunc = pFunc;
}

function<void(void)> HListener::GetFunc() const
{
	return m_pFunc;
}
