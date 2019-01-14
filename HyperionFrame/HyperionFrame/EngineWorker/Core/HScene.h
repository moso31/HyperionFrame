#pragma once

#include "HBVH.h"
#include "HShape.h"
#include "HLine.h"
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

	// 初始化场景各参数信息
	void Init(ComPtr<ID3D12GraphicsCommandList> pCommandList);

	// 场景初始化完毕后需要刷新一次，以提供矩阵和加速结构信息。
	void InitSceneData();

	void Update(UINT8* pMappedConstantBuffer, const UINT alignedConstantBufferSize);
	void Render(ComPtr<ID3D12GraphicsCommandList> pCommandList, const map<string, ComPtr<ID3D12PipelineState>>& pPSOs, ComPtr<ID3D12DescriptorHeap> pCbvHeap, UINT cbvDescriptorSize);
	void Release() {}

	void OnMouseDown(int x, int y);
	void OnKeyDown(WPARAM wParam);

	Camera*						CreateCamera();
	HPointLight*				CreatePointLight();
	shared_ptr<HMatteMaterial>	CreateMatteMaterial(const XMCOLOR3& kd, const float sigma);
	shared_ptr<HMirrorMaterial> CreateMirrorMaterial(const XMCOLOR3& kr);
	shared_ptr<HGlassMaterial>	CreateGlassMaterial(const XMCOLOR3& Kr, const XMCOLOR3& Kt, const float eta);

	Camera*		GetMainCamera()					{ return m_mainCamera; }
	int			GetCommonFeatureTableCount()	{ return (int)m_sceneManager->GetCommonFeatureTableCount(); }
	AABB		GetAABB()						{ return m_aabb; }

	// 场景射线碰撞检测。
	// 返回较具体的内容，包括射线和相交处物体的具体信息。
	bool Intersect(Ray worldRay, SurfaceInteraction* out_isect, int* out_hitShapeIndex, float tMax = FLT_MAX) const;
	
	// 场景射线碰撞检测（快速）。
	// 仅探测到是否发生碰撞即返回结果。
	bool IntersectP(Ray worldRay) const;

	// BMP图片生成函数。
	void MakeBMPImage();
	void MakeBMPImageTile(int tileX, int tileY, XMINT2 tileSize, int tileSampleCount, ImageBMPData* pRGB);

public:
	// 下列参数负责记录场景内常用的交互数据信息。
	// 和其他类相互结合使用的情况比较多见，故直接暴露在外。
	vector<Transform*>	transformNodes;
	vector<Camera*>		cameras;
	vector<HLight*>		lights;
	vector<shared_ptr<HPrimitive>>		primitives;
	vector<shared_ptr<HLine>>			debugMsgLines;
	vector<shared_ptr<HMaterial>>		materials;

private:
	// 构建并生成BVH树。
	void UpdateAccelerateStructure();

	void CreateRayTracingLine();

private:
	std::shared_ptr<DXResource>		m_dxResources;
	std::shared_ptr<HSceneManager>	m_sceneManager;

	Camera*				m_mainCamera;
	AABB				m_aabb;
	HBVHTree*			m_bvhTree;

	int					m_makingProcessIndex;
	CBufferEyePos		m_cbEyePos;
};
