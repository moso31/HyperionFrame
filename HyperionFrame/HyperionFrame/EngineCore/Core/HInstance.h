#pragma once
#include <memory>
using namespace std;

template <typename T>
class HInstance
{
public:
	HInstance() {}
	virtual ~HInstance() {}

	static shared_ptr<T> GetInstance()
	{
		if (!s_instance) s_instance = make_shared<T>();
		return s_instance;
	}

protected:
	static shared_ptr<T> s_instance;
};

template <typename T>
shared_ptr<T> HInstance<T>::s_instance;