#pragma once
#include "HLine.h"

class HSegment : public HLine
{
public:
	HSegment();
	HSegment(const shared_ptr<DXResource>& dxResources);
	~HSegment();

	ePrimitiveType GetType() { return SEGMENT; }

	void InitParameters(XMFLOAT3 point1, XMFLOAT3 point2);

	void Update();
	void Render(ComPtr<ID3D12GraphicsCommandList> pCommandList);
};
