#include "HSFirstPersonalCamera.h"
#include "Camera.h"

HSFirstPersonalCamera::HSFirstPersonalCamera(shared_ptr<HObject> pObject) :
	HScript(pObject),
	m_fMoveSpeed(0.3f),
	m_fSensitivity(0.01f)
{
	m_pCamera = dynamic_pointer_cast<Camera>(pObject);
	memset(m_bMoveState, false, sizeof(m_bMoveState));
}

void HSFirstPersonalCamera::Update()
{
	XMFLOAT3 pos = m_pCamera->GetTranslation();
	XMVECTOR posV = XMLoadFloat3(&pos);
	XMFLOAT3 fw = m_pCamera->GetForward();
	XMVECTOR fwV = XMLoadFloat3(&fw);
	XMFLOAT3 left = m_pCamera->GetLeft();
	XMVECTOR leftV = XMLoadFloat3(&left);

	XMVECTOR moveCommandV = { 0.0f, 0.0f, 0.0f, 0.0f };
	if (m_bMoveState[POSITIVE_Z]) moveCommandV += fwV;
	if (m_bMoveState[NEGATIVE_Z]) moveCommandV -= fwV;
	if (m_bMoveState[POSITIVE_X]) moveCommandV += leftV;
	if (m_bMoveState[NEGATIVE_X]) moveCommandV -= leftV;
	
	XMFLOAT3 result;
	XMStoreFloat3(&result, posV + moveCommandV * m_fMoveSpeed);
	m_pCamera->SetTranslation(result.x, result.y, result.z);
}

void HSFirstPersonalCamera::OnKeyDown(HEventArg eArg)
{
	if (eArg.VKey == 'W') m_bMoveState[POSITIVE_Z] = true;
	if (eArg.VKey == 'S') m_bMoveState[NEGATIVE_Z] = true;
	if (eArg.VKey == 'D') m_bMoveState[POSITIVE_X] = true;
	if (eArg.VKey == 'A') m_bMoveState[NEGATIVE_X] = true;
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
	rot.y -= (float)eArg.LastX * m_fSensitivity;	// yaw
	rot.x += (float)eArg.LastY * m_fSensitivity;	// pitch
	m_pCamera->SetRotation(rot.x, rot.y, rot.z);
}
