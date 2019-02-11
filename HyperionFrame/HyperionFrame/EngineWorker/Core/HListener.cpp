#include "HListener.h"

HListener::HListener(const shared_ptr<HObject>& pObject, const shared_ptr<function<void(void)>>& pFunc) : 
	m_pObject(pObject),
	m_pFunc(pFunc)
{
}
