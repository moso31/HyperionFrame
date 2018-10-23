#pragma once
#include "Box.h"
#include "HPointLight.h"
#include "HMatteMaterial.h"

class HScene : public HListener
{
public:
	static const UINT c_alignedConstantBufferSize = (sizeof(ModelViewProjectionConstantBuffer) + 255) & ~255; // ������������С���붼�� 256 �ֽڵ���������

	HScene();
	HScene(const std::shared_ptr<DXResource>& dxResource);
	~HScene();

	void OnResize();
	void Init(ComPtr<ID3D12GraphicsCommandList> pCommandList);
	void Update(UINT8* pMappedConstantBuffer);
	void Render(ComPtr<ID3D12GraphicsCommandList> pCommandList, ComPtr<ID3D12DescriptorHeap> pCbvHeap, UINT cbvDescriptorSize);
	void Release() {}

	void OnLButtonClicked(XMINT2 screenXY);
	void OnListen(WPARAM wParam);

	Camera* CreateCamera();
	Box* CreateBox(ComPtr<ID3D12GraphicsCommandList> pCommandList);
	HPointLight* CreatePointLight();
	HMatteMaterial* CreateMatteMaterial(const XMCOLOR3& kd, const float sigma);

	Camera* GetMainCamera() { return m_mainCamera; }

	int GetShapeCount() { return (int)m_shapes.size(); }

private:
	std::shared_ptr<DXResource> m_dxResources;

	vector<Transform*> m_transformNodes;
	vector<Camera*> m_cameras;
	vector<HLight*> m_lights;
	vector<Shape*> m_shapes;

	vector<HMaterial*> m_materials;

	Camera* m_mainCamera;
};
