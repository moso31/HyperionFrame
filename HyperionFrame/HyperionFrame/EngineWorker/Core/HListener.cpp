#include "HListener.h"

HListener::HListener(const shared_ptr<HObject>& pObject, const function<void(HEventArg)>& pFunc) :
	m_pObject(pObject),
	m_pFunc(pFunc)
{
}

void HListener::SetFunc(const function<void(HEventArg)>& pFunc)
{
	m_pFunc = pFunc;
}

function<void(HEventArg)> HListener::GetFunc() const
{
	return m_pFunc;
}
