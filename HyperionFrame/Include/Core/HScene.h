#pragma once
#include "Box.h"
#include "HPointLight.h"
#include "HMatteMaterial.h"

class HScene
{
public:
	static const UINT c_alignedConstantBufferSize = (sizeof(ModelViewProjectionConstantBuffer) + 255) & ~255; // 常量缓冲区大小必须都是 256 字节的整数倍。

	HScene();
	HScene(const std::shared_ptr<DXResource>& dxResource);
	~HScene();

	void OnResize();
	void Init(ComPtr<ID3D12GraphicsCommandList> pCommandList);
	void Update(UINT8* pMappedConstantBuffer);
	void Render(ComPtr<ID3D12GraphicsCommandList> pCommandList, ComPtr<ID3D12DescriptorHeap> pCbvHeap, UINT cbvDescriptorSize);
	void Release() {}

	void OnLButtonClicked(XMINT2 screenXY);

	Camera* CreateCamera();
	Box* CreateBox(ComPtr<ID3D12GraphicsCommandList> pCommandList);
	HPointLight* CreatePointLight();

	Camera* GetMainCamera() { return m_mainCamera; }

	int GetShapeCount() { return m_shapes.size(); }

private:
	std::shared_ptr<DXResource> m_dxResources;

	vector<Transform*> m_transformNodes;
	vector<Camera*> m_cameras;
	vector<HLight*> m_lights;
	vector<Shape*> m_shapes;

	vector<HMaterial*> m_materials;

	Camera* m_mainCamera;
};
