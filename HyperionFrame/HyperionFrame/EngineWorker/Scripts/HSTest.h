#pragma once
#include "HScript.h"

class HSTest : public HScript
{
public:
	HSTest(shared_ptr<HObject> pObject);
	void Update();

private:
	float m_fRotationY;
};
