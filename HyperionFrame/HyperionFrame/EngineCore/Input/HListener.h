#pragma once

class HListener
{
public:
	HListener() {}
	~HListener() {}

	virtual void OnNotify() = 0;

private:

};