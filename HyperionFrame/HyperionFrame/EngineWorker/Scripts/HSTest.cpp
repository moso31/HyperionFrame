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
	HFloat3 rot = pTransformData->GetRotation();
	rot.y += 0.05f;
	pTransformData->SetRotation(rot.x, rot.y, rot.z);
}
