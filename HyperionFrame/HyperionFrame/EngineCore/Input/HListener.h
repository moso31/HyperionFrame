#pragma once

typedef unsigned int UINT;

class HListener
{
public:
	HListener() {}
	~HListener() {}

	virtual void OnMouseDown(UINT x, UINT y) {}
	virtual void OnKeyDown() {}
	virtual void OnKeyUp() {}

private:

};