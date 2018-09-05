#pragma once
#include "header.h"

inline void ThrowIfFailed(HRESULT hr)
{
	if (FAILED(hr))
	{
		// �ڴ��������öϵ㣬�Բ��� Win32 API ����
		throw (hr);
	}
}

// �������������԰������ԡ�
#if defined(_DEBUG)
inline void SetName(ID3D12Object* pObject, LPCWSTR name)
{
	pObject->SetName(name);
}
#else
inline void SetName(ID3D12Object*, LPCWSTR)
{
}
#endif

// ��ʹ�����豸�޹ص�����(DIP)��ʾ�ĳ���ת��Ϊʹ���������ر�ʾ�ĳ��ȡ�
inline float ConvertDipsToPixels(float dips, float dpi)
{
	static const float dipsPerInch = 96.0f;
	return floorf(dips * dpi / dipsPerInch + 0.5f); // ���뵽��ӽ���������
}

// Ϊ ComPtr<T> ���� helper ������
// ����������ָ��Ϊ�������ơ�
#define NAME_D3D12_OBJECT(x) SetName(x.Get(), L#x)