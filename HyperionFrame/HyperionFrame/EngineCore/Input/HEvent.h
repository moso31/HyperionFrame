#pragma once
#include <vector>
#include <memory>

#include "HListener.h"

using namespace std;


class HEvent
{
public:
	HEvent() {}
	~HEvent() {}

	void AddListener(shared_ptr<HListener> pListener);
	void Notify();

private:
	vector<shared_ptr<HListener>> m_listener;
};