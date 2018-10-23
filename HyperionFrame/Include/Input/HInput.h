#pragma once
#include "header.h"

class HListener
{
public:
	HListener() {}
	virtual ~HListener() {}

	virtual void OnListen(WPARAM wParam) {}

private:
	
};

class HInput
{
public:
	HInput() {}
	~HInput() {}

	void Attach(HListener* listener) 
	{ 
		m_inputList.push_back(listener); 
	}

	void Notify(WPARAM wParam)
	{
		for each (auto listener in m_inputList)
		{
			listener->OnListen(wParam);
		}
	}

private:
	vector<HListener*> m_inputList;
};
