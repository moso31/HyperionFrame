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

	shared_ptr<Box>			CreateBox(string name, HFloat width = 1.0f, HFloat height = 1.0f, HFloat depth = 1.0f);
	shared_ptr<Sphere>		CreateSphere(string name, HFloat radius, HInt segmentHorizontal, HInt segmentVertical);
	shared_ptr<HMesh>		CreateMesh(string name, string filepath);
	shared_ptr<HSegment>	CreateSegment(string name, HFloat3 point1, HFloat3 point2);

	shared_ptr<Camera>				CreateCamera();
	shared_ptr<HPointLight>			CreatePointLight();
	shared_ptr<HPBRMaterialMatte>		CreateMatteMaterial(const HFloat3& kd, const HFloat sigma);
	shared_ptr<HPBRMaterialMirror>		CreateMirrorMaterial(const HFloat3& kr);
	shared_ptr<HPBRMaterialGlass>		CreateGlassMaterial(const HFloat3& Kr, const HFloat3& Kt, const HFloat eta);

	shared_ptr<HScript>		CreateScript(const HSCRIPTTYPE scriptType, const shared_ptr<HObject>& pObject);

	shared_ptr<HListener>	AddEventListener(const HEVENTTYPE eventType, const shared_ptr<HObject>& pObject, const function<void(HEventArg)>& pFunc);

private:
	shared_ptr<DXResource>	m_dxResources;
	shared_ptr<HScene>		m_pTargetScene;
};
