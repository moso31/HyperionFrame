#pragma once
#include <vector>

#include "HInstance.h"
#include "HListener.h"

typedef unsigned int UINT;

class HEventOnMouseDown : public HInstance<HEventOnMouseDown>
{
public:
	HEventOnMouseDown() {}
	virtual ~HEventOnMouseDown() {}

	void AddListener(shared_ptr<HListener> pListener);
	void Notify(UINT x, UINT y);

protected:
	vector<shared_ptr<HListener>> m_listener;
};

class HEventOnKeyDown : public HInstance<HEventOnKeyDown>
{
public:
	HEventOnKeyDown() {}
	virtual ~HEventOnKeyDown() {}

	void AddListener(shared_ptr<HListener> pListener);
	void Notify();

protected:
	vector<shared_ptr<HListener>> m_listener;
};

class HEventOnKeyUp : public HInstance<HEventOnKeyUp>
{
public:
	HEventOnKeyUp() {}
	virtual ~HEventOnKeyUp() {}

	void AddListener(shared_ptr<HListener> pListener);
	void Notify();

protected:
	vector<shared_ptr<HListener>> m_listener;
};
