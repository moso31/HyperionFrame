#pragma once

#include "HBVH.h"
#include "Shape.h"
#include "Camera.h"
#include "HMaterial.h"
#include "HLight.h"
#include "ImageGenerator.h"
#include "HSceneManager.h"

class HScene : public HListener
{
public:
	HScene();
	HScene(const std::shared_ptr<DXResource>& dxResource);
	~HScene();

	void OnResize();
	void Init(ComPtr<ID3D12GraphicsCommandList> pCommandList);
	void InitSceneData();
	void Update(UINT8* pMappedConstantBuffer, const UINT alignedConstantBufferSize);
	void Render(ComPtr<ID3D12GraphicsCommandList> pCommandList, ComPtr<ID3D12DescriptorHeap> pCbvHeap, UINT cbvDescriptorSize);
	void Release() {}

	void OnMouseDown(int x, int y);
	void OnKeyDown(WPARAM wParam);

	Camera*						CreateCamera();
	HPointLight*				CreatePointLight();
	shared_ptr<HMatteMaterial>	CreateMatteMaterial(const XMCOLOR3& kd, const float sigma);
	shared_ptr<HMirrorMaterial> CreateMirrorMaterial(const XMCOLOR3& kr);
	shared_ptr<HGlassMaterial>	CreateGlassMaterial(const XMCOLOR3& Kr, const XMCOLOR3& Kt, const float eta);

	Camera*		GetMainCamera()				{ return m_mainCamera; }
	int			GetCommonFeatureTableCount()	{ return (int)m_sceneManager->GetCommonFeatureTableCount(); }
	AABB		GetAABB()					{ return m_aabb; }

	bool Intersect(Ray worldRay, SurfaceInteraction* out_isect, int* out_hitShapeIndex, float tMax = FLT_MAX) const;
	bool IntersectP(Ray worldRay) const;

	void MakeImageTile(int tileX, int tileY, XMINT2 tileSize, int tileSampleCount, ImageBMPData* pRGB);

public:
	vector<Transform*>	transformNodes;
	vector<Camera*>		cameras;
	vector<HLight*>		lights;
	vector<shared_ptr<HPrimitive>>		primitives;
	vector<shared_ptr<HMaterial>>		materials;

private:
	void UpdateAccelerateStructure();

private:
	std::shared_ptr<DXResource>		m_dxResources;
	std::shared_ptr<HSceneManager>	m_sceneManager;

	Camera*				m_mainCamera;
	AABB				m_aabb;
	HBVHTree*			m_bvhTree;

	int					m_makingProcessIndex;
	CBufferEyePos		m_cbEyePos;
};
