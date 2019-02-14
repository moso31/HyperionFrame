#pragma once
#include "HScript.h"

class HSFirstPersonalCamera : public HScript
{
public:
	HSFirstPersonalCamera(shared_ptr<HObject> pObject);
	void Update();

	void OnKeyDown(HEventArg eArg);
	void OnKeyUp(HEventArg eArg);
	void OnMouseDown(HEventArg eArg);
	void OnMouseMove(HEventArg eArg);

private:
	shared_ptr<Camera> m_pCamera;
	XMFLOAT3 m_command;
};
