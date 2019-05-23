#pragma once
#include "HScript.h"

#define POSITIVE_X 0
#define POSITIVE_Y 1
#define POSITIVE_Z 2
#define NEGATIVE_X 3
#define NEGATIVE_Y 4
#define NEGATIVE_Z 5

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
	HFloat m_fMoveSpeed;
	HFloat m_fSensitivity;

	// +X, +Y, +Z, -X, -Y, -Z
	bool m_bMoveState[6]; 
};
