#include "HEvent.h"

void HEvent::AddListener(shared_ptr<HListener> pListener)
{
	m_listener.push_back(pListener);
}

void HEvent::Notify()
{
	if (m_listener.empty())
		return;

	printf("HEvent::Notify() processed.\n");
	for (auto it = m_listener.begin(); it != m_listener.end(); it++)
	{
		(*it)->OnNotify();
	}
}
