#pragma once
#include "header.h"

class HListener
{
public:
	HListener() {}
	virtual ~HListener() {}

	virtual void OnMouseDown(int x, int y) {}
	virtual void OnKeyDown(WPARAM wParam) {}

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

	void OnMouseDown(int x, int y);
	void OnKeyDown(WPARAM wParam);

private:
	vector<HListener*> m_inputList;
};
