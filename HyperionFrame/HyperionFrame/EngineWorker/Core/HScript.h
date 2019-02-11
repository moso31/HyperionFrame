#pragma once
#include "header.h"
#include "HScriptType.h"

class HScript
{
public:
	HScript(const shared_ptr<HObject>& pObject);
	virtual ~HScript() {}

	void SetObject(shared_ptr<HObject>& pObject) { m_pObject = pObject; }

	virtual void Update() {};

protected:
	shared_ptr<HObject> m_pObject;
};