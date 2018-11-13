#pragma once
#include "Box.h"
#include "Sphere.h"
#include "HPointLight.h"
#include "ImageGenerator.h"

#include "HMatteMaterial.h"
#include "HMirrorMaterial.h"
#include "HGlassMaterial.h"

class HScene : public HListener
{
public:
	static const UINT c_alignedConstantBufferSize = (sizeof(ModelViewProjectionConstantBuffer) + 255) & ~255; // ������������С���붼�� 256 �ֽڵ���������

	HScene();
	HScene(const std::shared_ptr<DXResource>& dxResource);
	~HScene();

	void OnResize();
	void Init(ComPtr<ID3D12GraphicsCommandList> pCommandList);
	void InitSceneData();
	void Update(UINT8* pMappedConstantBuffer);
	void Render(ComPtr<ID3D12GraphicsCommandList> pCommandList, ComPtr<ID3D12DescriptorHeap> pCbvHeap, UINT cbvDescriptorSize);
	void Release() {}

	void OnMouseDown(int x, int y);
	void OnKeyDown(WPARAM wParam);

	Camera* CreateCamera();
	Box* CreateBox(ComPtr<ID3D12GraphicsCommandList> pCommandList);
	Sphere* CreateSphere(ComPtr<ID3D12GraphicsCommandList> pCommandList, float radius = 1.0f, int segmentVertical = 16.0f, int segmentHorizontal = 16.0f);
	HPointLight* CreatePointLight();
	HMatteMaterial* CreateMatteMaterial(const XMCOLOR3& kd, const float sigma);
	HMirrorMaterial* CreateMirrorMaterial(const XMCOLOR3& kr);
	HGlassMaterial* CreateGlassMaterial(const XMCOLOR3& Kr, const XMCOLOR3& Kt, const float eta);

	Camera* GetMainCamera() { return m_mainCamera; }
	int GetShapeCount()		{ return (int)shapes.size(); }
	AABB GetAABB()			{ return m_aabb; }

	void MakeImageTile(int tileX, int tileY, XMINT2 tileSize, int tileSampleCount, ImageBMPData* pRGB);

public:
	vector<Transform*>	transformNodes;
	vector<Camera*>		cameras;
	vector<HLight*>		lights;
	vector<Shape*>		shapes;
	vector<HMaterial*>	materials;

private:
	void UpdateAABB();

private:
	std::shared_ptr<DXResource> m_dxResources;
	Camera* m_mainCamera;
	AABB	m_aabb;

	int m_makingProcessIndex;
};
