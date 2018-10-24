#pragma once
#include "Box.h"
#include "HPointLight.h"
#include "HMatteMaterial.h"

class HScene : public HListener
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

	void OnMouseDown(int x, int y);
	void OnKeyDown(WPARAM wParam);

	Camera* CreateCamera();
	Box* CreateBox(ComPtr<ID3D12GraphicsCommandList> pCommandList);
	HPointLight* CreatePointLight();
	HMatteMaterial* CreateMatteMaterial(const XMCOLOR3& kd, const float sigma);

	Camera* GetMainCamera() { return m_mainCamera; }
	int GetShapeCount() { return (int)shapes.size(); }

public:
	vector<Transform*> transformNodes;
	vector<Camera*> cameras;
	vector<HLight*> lights;
	vector<Shape*> shapes;

	vector<HMaterial*> materials;

private:
	std::shared_ptr<DXResource> m_dxResources;
	Camera* m_mainCamera;
};
