#include "HSFirstPersonalCamera.h"
#include "Camera.h"

HSFirstPersonalCamera::HSFirstPersonalCamera(shared_ptr<HObject> pObject) :
	HScript(pObject)
{
	m_pCamera = dynamic_pointer_cast<Camera>(pObject);
}

void HSFirstPersonalCamera::Update()
{
}

void HSFirstPersonalCamera::OnKeyDown(HEventArg eArg)
{
}

void HSFirstPersonalCamera::OnKeyUp(HEventArg eArg)
{
}

void HSFirstPersonalCamera::OnMouseDown(HEventArg eArg)
{
}

void HSFirstPersonalCamera::OnMouseMove(HEventArg eArg)
{
	auto rot = m_pCamera->GetRotation();
	rot.y -= (float)eArg.LastX * 0.005f;	// yaw
	rot.x += (float)eArg.LastY * 0.005f;	// pitch
	m_pCamera->SetRotation(rot.x, rot.y, rot.z);
}
