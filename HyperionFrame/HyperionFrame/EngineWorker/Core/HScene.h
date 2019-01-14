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

	// ��ʼ��������������Ϣ
	void Init(ComPtr<ID3D12GraphicsCommandList> pCommandList);

	// ������ʼ����Ϻ���Ҫˢ��һ�Σ����ṩ����ͼ��ٽṹ��Ϣ��
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

	// ����������ײ��⡣
	// ���ؽϾ�������ݣ��������ߺ��ཻ������ľ�����Ϣ��
	bool Intersect(Ray worldRay, SurfaceInteraction* out_isect, int* out_hitShapeIndex, float tMax = FLT_MAX) const;
	
	// ����������ײ��⣨���٣���
	// ��̽�⵽�Ƿ�����ײ�����ؽ����
	bool IntersectP(Ray worldRay) const;

	// BMPͼƬ���ɺ�����
	void MakeBMPImage();
	void MakeBMPImageTile(int tileX, int tileY, XMINT2 tileSize, int tileSampleCount, ImageBMPData* pRGB);

public:
	// ���в��������¼�����ڳ��õĽ���������Ϣ��
	// ���������໥���ʹ�õ�����Ƚ϶������ֱ�ӱ�¶���⡣
	vector<Transform*>	transformNodes;
	vector<Camera*>		cameras;
	vector<HLight*>		lights;
	vector<shared_ptr<HPrimitive>>		primitives;
	vector<shared_ptr<HLine>>			debugMsgLines;
	vector<shared_ptr<HMaterial>>		materials;

private:
	// ����������BVH����
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
