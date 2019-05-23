#pragma once
#include "HScript.h"

class HSTest : public HScript
{
public:
	HSTest(shared_ptr<HObject> pObject);
	void Update();

private:
	HFloat m_fRotationY;
};
