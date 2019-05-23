#pragma once
#include "header.h"

namespace DX
{
	inline void ThrowIfFailed(HRESULT hr)
	{
		if (FAILED(hr))
		{
			// 在此行中设置断点，以捕获 Win32 API 错误。
			throw (hr);
		}
	}

	// 向对象分配名称以帮助调试。
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

	// 将使用与设备无关的像素(DIP)表示的长度转换为使用物理像素表示的长度。
	inline HFloat ConvertDipsToPixels(HFloat dips, HFloat dpi)
	{
		static const HFloat dipsPerInch = 96.0f;
		return floorf(dips * dpi / dipsPerInch + 0.5f); // 舍入到最接近的整数。
	}

	// 为 ComPtr<T> 命名 helper 函数。
	// 将变量名称指定为对象名称。
#define NAME_D3D12_OBJECT(x) SetName(x.Get(), L#x)
}