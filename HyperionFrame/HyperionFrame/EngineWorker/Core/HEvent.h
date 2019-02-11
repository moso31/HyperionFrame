#pragma once
#include "HListener.h"
#include "HInstance.h"

class HEvent
{
public:
	HEvent() {};
	virtual ~HEvent() {};

	virtual void AddListener(const shared_ptr<HListener>& pListener);
	void OnNotify();

protected:
	vector<shared_ptr<HListener>> m_listeners;
};

class HKeyDownEvent : public HEvent, public HInstance<HKeyDownEvent>
{
public:
	void AddListener(const shared_ptr<HListener>& pListener);

private:

};