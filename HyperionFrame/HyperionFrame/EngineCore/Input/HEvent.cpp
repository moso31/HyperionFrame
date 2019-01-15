#include "HEvent.h"

void HEventOnMouseDown::AddListener(shared_ptr<HListener> pListener)
{
	GetInstance()->m_listener.push_back(pListener);
}

void HEventOnMouseDown::Notify(UINT x, UINT y)
{
	auto vListener = GetInstance()->m_listener;
	if (vListener.empty())
		return;

	for (auto it = vListener.begin(); it != vListener.end(); it++)
	{
		(*it)->OnMouseDown(x, y);
	}
}

void HEventOnKeyDown::AddListener(shared_ptr<HListener> pListener)
{
	GetInstance()->m_listener.push_back(pListener);
}

void HEventOnKeyDown::Notify()
{
	auto vListener = GetInstance()->m_listener;
	if (vListener.empty())
		return;

	for (auto it = vListener.begin(); it != vListener.end(); it++)
	{
		(*it)->OnKeyDown();
	}
}

void HEventOnKeyUp::AddListener(shared_ptr<HListener> pListener)
{
	GetInstance()->m_listener.push_back(pListener);
}

void HEventOnKeyUp::Notify()
{
	auto vListener = GetInstance()->m_listener;
	if (vListener.empty())
		return;

	for (auto it = vListener.begin(); it != vListener.end(); it++)
	{
		(*it)->OnKeyUp();
	}
}
