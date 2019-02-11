#pragma once
#include <string>
#include <vector>
#include <memory>

#include "HListener.h"

using namespace std;

class HObject
{
public:
	HObject() {}
	virtual ~HObject() {}

	string GetName();
	void SetName(string name);

protected:
	string name;
};
