#pragma once
#include "HPrimitive.h"
#include "HEvent.h"

class HSceneManager
{
public:
	HSceneManager();
	HSceneManager(std::shared_ptr<DXResource> dxResources, const shared_ptr<HScene>& pTargetScene);
	~HSceneManager();

	// 设置目标场景。
	// 之后所有的场景管理操作都会在此场景中进行。
	void SetTargetScene(shared_ptr<HScene> pTargetScene);

	shared_ptr<Box>			CreateBox(string name, float width = 1.0f, float height = 1.0f, float depth = 1.0f);
	shared_ptr<Sphere>		CreateSphere(string name, float radius, int segmentHorizontal, int segmentVertical);
	shared_ptr<HMesh>		CreateMesh(string name, string filepath);
	shared_ptr<HSegment>	CreateSegment(string name, XMFLOAT3 point1, XMFLOAT3 point2);

	shared_ptr<Camera>				CreateCamera();
	shared_ptr<HPointLight>			CreatePointLight();
	shared_ptr<HMatteMaterial>		CreateMatteMaterial(const XMCOLOR3& kd, const float sigma);
	shared_ptr<HMirrorMaterial>		CreateMirrorMaterial(const XMCOLOR3& kr);
	shared_ptr<HGlassMaterial>		CreateGlassMaterial(const XMCOLOR3& Kr, const XMCOLOR3& Kt, const float eta);

	shared_ptr<HScript>		CreateScript(const HSCRIPTTYPE scriptType, const shared_ptr<HObject>& pObject);

	shared_ptr<HListener>	AddEventListener(const HEVENTTYPE eventType, const shared_ptr<HObject>& pObject, const function<void(HEventArg)>& pFunc);

private:
	shared_ptr<DXResource>	m_dxResources;
	shared_ptr<HScene>		m_pTargetScene;
};
