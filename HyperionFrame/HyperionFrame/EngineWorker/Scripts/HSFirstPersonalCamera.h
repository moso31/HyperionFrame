#pragma once
#include "HScript.h"

class HSFirstPersonalCamera : public HScript
{
public:
	HSFirstPersonalCamera(shared_ptr<HObject> pObject);
	void Update();

	void KeyDownTest(HEventArg eArg) { printf("tested\n"); }

private:

};
