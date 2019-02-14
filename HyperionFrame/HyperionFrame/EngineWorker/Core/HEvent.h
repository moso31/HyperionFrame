#pragma once
#include "HListener.h"
#include "HInstance.h"

enum HEVENTTYPE
{
	HEVENT_NONE,
	HEVENT_KEYDOWN,
	HEVENT_KEYUP,
	HEVENT_MOUSEDOWN,
	HEVENT_MOUSEUP,
	HEVENT_MOUSEMOVE
};

class HEvent
{
public:
	HEvent() : m_type(HEVENT_NONE) {}
	virtual ~HEvent() {}

	virtual void AddListener(const shared_ptr<HListener>& pListener);
	void OnNotify(HEventArg eArg);

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
};

class HKeyUpEvent : public HEvent, public HInstance<HKeyUpEvent>
{
public:
	HKeyUpEvent() { m_type = HEVENT_KEYUP; }
	~HKeyUpEvent() {}
};

class HMouseDownEvent : public HEvent, public HInstance<HMouseDownEvent>
{
public:
	HMouseDownEvent() { m_type = HEVENT_MOUSEDOWN; }
	~HMouseDownEvent() {}
};

class HMouseUpEvent : public HEvent, public HInstance<HMouseUpEvent>
{
public:
	HMouseUpEvent() { m_type = HEVENT_MOUSEUP; }
	~HMouseUpEvent() {}
};

class HMouseMoveEvent : public HEvent, public HInstance<HMouseMoveEvent>
{
public:
	HMouseMoveEvent() { m_type = HEVENT_MOUSEMOVE; }
	~HMouseMoveEvent() {}
};