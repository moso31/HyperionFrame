#pragma once

#include "HBVH.h"
#include "HShape.h"
#include "HLine.h"
#include "Camera.h"
#include "HMaterial.h"
#include "HLight.h"
#include "ImageGenerator.h"
#include "HSceneManager.h"

#include "HEvent.h"

#include "HSTest.h"

class HScene : public HListener, public enable_shared_from_this<HScene>
{
public:
	HScene();
	HScene(const std::shared_ptr<DXResource>& dxResource);
	~HScene();

	void OnResize();

	// ��ʼ��������������Ϣ
	void Init(ComPtr<ID3D12GraphicsCommandList> pCommandList);

	// ��ʼ����Ⱦ����Ϣ����Ҫ�����������ѵ����ݡ�
	void InitRendererData(ComPtr<ID3D12GraphicsCommandList> pCommandList);

	// ��ʼ��ͼԪ��Ϣ��
	void InitPrimitiveData();

	// ��ʼ���ṹ��Ϣ����Ҫ������primitive����֮�󣬵�һ�θ���ǰִ��һ�Σ����ṩTransform��BVH���ٽṹ��Ϣ��
	void InitStructureData();

	void Update(ComPtr<ID3D12GraphicsCommandList> pCommandList);
	void Render(ComPtr<ID3D12GraphicsCommandList> pCommandList, const map<string, ComPtr<ID3D12PipelineState>>& pPSOs);
	void Release() {}

	void OnMouseDown(UINT x, UINT y);
	void OnKeyDown();
	void OnKeyUp();

	shared_ptr<Box>			CreateBox(string name, float width = 1.0f, float height = 1.0f, float depth = 1.0f);
	shared_ptr<Sphere>		CreateSphere(string name, float radius, int segmentHorizontal, int segmentVertical);
	shared_ptr<HMesh>		CreateMesh(string filepath);
	shared_ptr<HSegment>	CreateSegment(string name, XMFLOAT3 point1, XMFLOAT3 point2);

	shared_ptr<HScript>		CreateScript(const HSCRIPTTYPE scriptType, const shared_ptr<HObject>& pObject);

	Camera*						CreateCamera();
	HPointLight*				CreatePointLight();
	shared_ptr<HMatteMaterial>	CreateMatteMaterial(const XMCOLOR3& kd, const float sigma);
	shared_ptr<HMirrorMaterial> CreateMirrorMaterial(const XMCOLOR3& kr);
	shared_ptr<HGlassMaterial>	CreateGlassMaterial(const XMCOLOR3& Kr, const XMCOLOR3& Kt, const float eta);

	Camera*		GetMainCamera()					{ return m_mainCamera; }
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

	vector<shared_ptr<HScript>>			scripts;

private:
	// ���������������Ҫִ�д˷�����������������������
	void UpdateDescriptors();

	// ����������BVH����
	void UpdateAccelerateStructure();

	// ���𳡾���ͼԪ��Ϣ�� �ӳټ��ء�
	void UpdatePrimitive(ComPtr<ID3D12GraphicsCommandList> pCommandList);

	// ���½ű���Ϣ
	void UpdateScripts();

	// �������е�Transform��Ϣ��
	void UpdateTransform();

	// �������е�Buffer��Ϣ����Ҫ�ڸ���Transform֮����С�
	void UpdateConstantBuffer();

private:
	std::shared_ptr<DXResource>		m_dxResources;
	std::shared_ptr<HSceneManager>	m_sceneManager;

	Camera*				m_mainCamera;
	AABB				m_aabb;
	HBVHTree*			m_bvhTree;

	int					m_makingProcessIndex;
	
	// ���ڴ�ų�����primitive���������ѡ�
	ComPtr<ID3D12DescriptorHeap>	m_cbvHeap;
	UINT							m_cbvDescriptorSize;

	// Hyperion �еĳ���ʵʱ������õķ����� �ӳټ��ء�
	// ������������ָ�ǰ֡����ִ�У����ǽ��ɴ� Ԥ���б� ����洢��
	// Ԥ���б� �е����������һ֡UpdateTransform֮ǰ���ص������С�֮������Ԥ���б��Է��ظ���ӡ�
	// Hyperion �������� �ӳټ��� ������ԭ���ǣ�ֻ����ÿ֡ CommandList �ر�ʱ���ܶԳ����� Buffer ���б����
	vector<shared_ptr<HPrimitive>> m_prepareToLoadList;
};
