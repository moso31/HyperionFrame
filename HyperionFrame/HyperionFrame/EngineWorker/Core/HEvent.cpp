#include "HEvent.h"

void HEvent::AddListener(const shared_ptr<HListener>& pListener)
{
	m_listeners.push_back(pListener);
}

void HEvent::OnNotify(HEventArg eArg)
{
	for (auto it = m_listeners.begin(); it != m_listeners.end(); it++)
	{
		(*it)->GetFunc()(eArg);
	}
}
