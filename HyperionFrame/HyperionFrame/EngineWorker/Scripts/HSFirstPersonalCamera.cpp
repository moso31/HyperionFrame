#include "HSFirstPersonalCamera.h"
#include "Camera.h"

HSFirstPersonalCamera::HSFirstPersonalCamera(shared_ptr<HObject> pObject) :
	HScript(pObject),
	m_fMoveSpeed(0.3f),
	m_fSensitivity(0.005f)
{
	m_pCamera = dynamic_pointer_cast<Camera>(pObject);
	memset(m_bMoveState, false, sizeof(m_bMoveState));
}

void HSFirstPersonalCamera::Update()
{
	HFloat3 pos = m_pCamera->GetTranslation();
	HFloat3 fw = m_pCamera->GetForward();
	HFloat3 right = m_pCamera->GetRight();

	HFloat3 moveCommandV(0.0f);
	if (m_bMoveState[POSITIVE_Z]) moveCommandV += fw;
	if (m_bMoveState[NEGATIVE_Z]) moveCommandV -= fw;
	if (m_bMoveState[POSITIVE_X]) moveCommandV += right;
	if (m_bMoveState[NEGATIVE_X]) moveCommandV -= right;
	
	HFloat3 result = pos + moveCommandV * m_fMoveSpeed;
	m_pCamera->SetTranslation(result.x, result.y, result.z);
}

void HSFirstPersonalCamera::OnKeyDown(HEventArg eArg)
{
	if (eArg.VKey == 'W') m_bMoveState[POSITIVE_Z] = true;
	if (eArg.VKey == 'S') m_bMoveState[NEGATIVE_Z] = true;
	if (eArg.VKey == 'D') m_bMoveState[POSITIVE_X] = true;
	if (eArg.VKey == 'A') m_bMoveState[NEGATIVE_X] = true;

	// 输出Camera当前transform信息
	if (eArg.VKey == 'P')
	{
		string name = m_pCamera->GetName();
		HFloat3 t = m_pCamera->GetTransform().GetTranslation();
		HFloat3 r = m_pCamera->GetTransform().GetRotation();
		printf("Camera info: name: %s translation: %.3f, %.3f, %.3f rotation: %.3f, %.3f, %.3f\n", 
			name.c_str(), t.x, t.y, t.z, r.x, r.y, r.z);
	}
}

void HSFirstPersonalCamera::OnKeyUp(HEventArg eArg)
{
	if (eArg.VKey == 'W') m_bMoveState[POSITIVE_Z] = false;
	if (eArg.VKey == 'S') m_bMoveState[NEGATIVE_Z] = false;
	if (eArg.VKey == 'D') m_bMoveState[POSITIVE_X] = false;
	if (eArg.VKey == 'A') m_bMoveState[NEGATIVE_X] = false;
}

void HSFirstPersonalCamera::OnMouseDown(HEventArg eArg)
{
}

void HSFirstPersonalCamera::OnMouseMove(HEventArg eArg)
{
	auto rot = m_pCamera->GetRotation();
	rot.y += (HFloat)eArg.LastX * m_fSensitivity;	// yaw
	rot.x -= (HFloat)eArg.LastY * m_fSensitivity;	// pitch
	m_pCamera->SetRotation(rot.x, rot.y, rot.z);
}
