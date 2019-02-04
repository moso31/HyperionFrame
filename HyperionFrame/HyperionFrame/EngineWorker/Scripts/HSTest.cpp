#include "HSTest.h"
#include "Transform.h"

HSTest::HSTest(shared_ptr<HObject> pObject) :
	HScript(pObject),
	m_fRotationY(0.0f)
{
}

void HSTest::Update()
{
	auto pTransformData = dynamic_pointer_cast<Transform>(m_pObject);
	pTransformData->SetRotation(0.0f, m_fRotationY, 0.0f);
	m_fRotationY += 0.01f;
}
