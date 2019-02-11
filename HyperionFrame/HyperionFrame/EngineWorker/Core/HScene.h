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

	// 初始化场景各参数信息
	void Init(ComPtr<ID3D12GraphicsCommandList> pCommandList);

	// 初始化渲染器信息，主要处理描述符堆的数据。
	void InitRendererData(ComPtr<ID3D12GraphicsCommandList> pCommandList);

	// 初始化图元信息。
	void InitPrimitiveData();

	// 初始化结构信息。需要在所有primitive创建之后，第一次更新前执行一次，以提供Transform和BVH加速结构信息。
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

	vector<shared_ptr<HScript>>			scripts;

private:
	// 在新生成物体后需要执行此方法，更新描述符的数量。
	void UpdateDescriptors();

	// 构建并生成BVH树。
	void UpdateAccelerateStructure();

	// 负责场景中图元信息的 延迟加载。
	void UpdatePrimitive(ComPtr<ID3D12GraphicsCommandList> pCommandList);

	// 更新脚本信息
	void UpdateScripts();

	// 更新所有的Transform信息。
	void UpdateTransform();

	// 更新所有的Buffer信息。需要在更新Transform之后进行。
	void UpdateConstantBuffer();

private:
	std::shared_ptr<DXResource>		m_dxResources;
	std::shared_ptr<HSceneManager>	m_sceneManager;

	Camera*				m_mainCamera;
	AABB				m_aabb;
	HBVHTree*			m_bvhTree;

	int					m_makingProcessIndex;
	
	// 用于存放场景内primitive的描述符堆。
	ComPtr<ID3D12DescriptorHeap>	m_cbvHeap;
	UINT							m_cbvDescriptorSize;

	// Hyperion 中的场景实时变更采用的方法是 延迟加载。
	// 即新添加物体的指令当前帧并不执行，而是交由此 预载列表 按序存储。
	// 预载列表 中的物体会在下一帧UpdateTransform之前加载到场景中。之后会清空预载列表以防重复添加。
	// Hyperion 采用这种 延迟加载 方案的原因是，只有在每帧 CommandList 关闭时才能对场景的 Buffer 进行变更。
	vector<shared_ptr<HPrimitive>> m_prepareToLoadList;
};
