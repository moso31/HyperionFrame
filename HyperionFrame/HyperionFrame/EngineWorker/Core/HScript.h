#pragma once
#include "header.h"

class HScript
{
public:
	HScript(shared_ptr<HObject>& pObject);
	virtual ~HScript() {}

	void SetObject(shared_ptr<HObject> pObject) { m_pObject = pObject; }

	virtual void Update() {};

protected:
	shared_ptr<HObject> m_pObject;
};