#pragma once
#include "HPrimitive.h"

class HLine : public HPrimitive
{
public:
	HLine();
	HLine(const shared_ptr<DXResource>& dxResources);
	virtual ~HLine();

	virtual void Update() = 0;
	virtual void Render(ComPtr<ID3D12GraphicsCommandList> pCommandList) = 0;

	virtual UINT GetAlignedConstantBufferSize();

protected:
	shared_ptr<DXResource>		m_dxResources;
};
