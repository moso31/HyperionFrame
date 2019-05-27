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

class HScene : public HObject, public enable_shared_from_this<HScene>
{
public:
	HScene() = default;
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

	void OnMouseDown(HEventArg eArg);
	void OnKeyDown(HEventArg eArg);
	void OnKeyUp(HEventArg eArg);

	shared_ptr<Camera>				GetMainCamera()					{ return m_mainCamera; }
	AABB							GetAABB()						{ return m_aabb; }

	// ����������ײ��⡣
	// ���ؽϾ�������ݣ��������ߺ��ཻ������ľ�����Ϣ��
	bool Intersect(Ray worldRay, SurfaceInteraction* out_isect, HInt* out_hitShapeIndex, HFloat tMax = FLT_MAX) const;
	
	// ����������ײ��⣨���٣���
	// ��̽�⵽�Ƿ�����ײ�����ؽ����
	bool IntersectP(Ray worldRay) const;

	// BMPͼƬ���ɺ�����
	void MakeBMPImage();
	void MakeBMPImageTile(HInt tileX, HInt tileY, HInt2 tileSize, HInt tileSampleCount, ImageBMPData* pRGB);

	// ���� ������������ ��Ϊ��Ԫ��
	// ��Ϊ ���������� �ڹ�����ʱ��ʹ����һЩ ������ ��˽�г�Ա������
	friend class HSceneManager;

public:
	// ���в��������¼�����ڳ��õĽ���������Ϣ��
	// ���������໥���ʹ�õ�����Ƚ϶������ֱ�ӱ�¶���⡣
	vector<shared_ptr<Camera>>			cameras;
	vector<shared_ptr<HLight>>			lights;
	vector<shared_ptr<HMaterial>>		materials;
	vector<shared_ptr<HPrimitive>>		primitives;
	vector<shared_ptr<HLine>>			debugMsgLines;
	vector<shared_ptr<HScript>>			scripts;

private:
	// ���������������Ҫִ�д˷�����������������������
	void UpdateDescriptors();

	// ����������BVH����
	void UpdateAccelerateStructure();

	// ���𳡾���ͼԪ��Ϣ�� �ӳټ��ء�
	void UpdatePrimitive(ComPtr<ID3D12GraphicsCommandList> pCommandList);

	// �ű�ִ��Update()��
	void UpdateScripts();

	// ����Transform��Ϣ��д���ͼԪ��
	void UpdateTransform();

	// �������е�Buffer��Ϣ����Ҫ�ڸ���Transform֮����С�
	void UpdateConstantBuffer();

private:
	std::shared_ptr<DXResource>		m_dxResources;
	std::shared_ptr<HSceneManager>	m_sceneManager;

	shared_ptr<Camera>				m_mainCamera;
	AABB				m_aabb;
	HBVHTree*			m_bvhTree;

	HInt					m_makingProcessIndex;
	
	// ���ڴ�ų�����primitive���������ѡ�
	ComPtr<ID3D12DescriptorHeap>	m_cbvHeap;
	HUInt							m_cbvDescriptorSize;

	// Hyperion �еĳ���ʵʱ������õķ����� �ӳټ��ء�
	// ������������ָ�ǰ֡����ִ�У����ǽ��ɴ� Ԥ���б� ����洢��
	// Ԥ���б� �е����������һ֡UpdateTransform֮ǰ���ص������С�֮������Ԥ���б��Է��ظ���ӡ�
	// Hyperion �������� �ӳټ��� ������ԭ���ǣ�ֻ����ÿ֡ CommandList �ر�ʱ���ܶԳ����� Buffer ���б����
	vector<shared_ptr<HPrimitive>> m_prepareToLoadList;
};
