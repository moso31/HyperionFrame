#pragma once
#include <string>

using namespace std;

class HObject
{
public:
	HObject() {}
	~HObject() {}

	string GetName();
	void SetName(string name);

private:
	string name;
};
