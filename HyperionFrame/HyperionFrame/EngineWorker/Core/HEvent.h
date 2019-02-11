#pragma once
#include "HListener.h"
#include "HInstance.h"

enum HEVENTTYPE
{
	HEVENT_NONE,
	HEVENT_KEYDOWN,
	HEVENT_KEYUP,
	HEVENT_MOUSEDOWN
};

class HEvent
{
public:
	HEvent() : m_type(HEVENT_NONE) {}
	virtual ~HEvent() {}

	virtual void AddListener(const shared_ptr<HListener>& pListener);
	void OnNotify();

	HEVENTTYPE GetType() const { return m_type; }
	void SetType(const HEVENTTYPE type) { m_type = type; }

protected:
	vector<shared_ptr<HListener>> m_listeners;
	HEVENTTYPE m_type;
};

class HKeyDownEvent : public HEvent, public HInstance<HKeyDownEvent>
{
public:
	HKeyDownEvent() { m_type = HEVENT_KEYDOWN; }
	~HKeyDownEvent() {}

private:

};