#pragma once
#include "DXResource.h"
#include "DirectXHelper.h"
#include "FileReader.h"
#include "ShaderStructures.h"

#include "Shape.h"

class Camera : public Shape
{
public:
	Camera();
	Camera(const shared_ptr<DXResource>& dxResources);
	~Camera();

	void Init();
	void OnResize();

	void Update();
	void Render();

	D3D12_RECT GetScissorRect() { return m_scissorRect; }

private:
	shared_ptr<DXResource> m_dxResources;

	D3D12_RECT											m_scissorRect;
};
