#include "HSceneManager.h"
#include "DirectXHelper.h"
#include "DDSTextureLoader12.h"
#include "WICTextureLoader12.h"

#include "HScene.h"

#include "Box.h"
#include "Sphere.h"
#include "HMesh.h"
#include "HSegment.h"
#include "Camera.h"
#include "HPointLight.h"

#include "HTexture.h"
#include "HMaterial.h"
#include "HPBRMaterialMatte.h"
#include "HPBRMaterialGlass.h"
#include "HPBRMaterialMirror.h"

#include "HScriptType.h"
#include "HSTest.h"
#include "HSFirstPersonalCamera.h"

HSceneManager::HSceneManager(std::shared_ptr<DXResource> dxResources, const shared_ptr<HScene>& pTargetScene) :
	m_dxResources(dxResources),
	m_pTargetScene(pTargetScene), 
	m_descriptorCount(0)
{
}

HSceneManager::~HSceneManager()
{
}

void HSceneManager::SetTargetScene(shared_ptr<HScene> pTargetScene)
{
	m_pTargetScene = pTargetScene;
}

shared_ptr<Box> HSceneManager::CreateBox(string name, HFloat width, HFloat height, HFloat depth)
{
	auto box = make_shared<Box>(m_dxResources);
	box->InitParameters();
	box->SetName(name);

	m_pTargetScene->primitives.push_back(box);
	m_pTargetScene->m_preLoadList.push_back(box);

	UpdateDescriptorCount_CreateShape();
	return box;
}

shared_ptr<Sphere> HSceneManager::CreateSphere(string name, HFloat radius, HInt segmentHorizontal, HInt segmentVertical)
{
	auto sphere = make_shared<Sphere>(m_dxResources);
	sphere->InitParameters(radius, segmentHorizontal, segmentVertical);
	sphere->SetName(name);

	m_pTargetScene->primitives.push_back(sphere);
	m_pTargetScene->m_preLoadList.push_back(sphere);

	UpdateDescriptorCount_CreateShape();
	return sphere;
}

shared_ptr<HMesh> HSceneManager::CreateMesh(string name, string filepath)
{
	auto mesh = make_shared<HMesh>(m_dxResources);
	mesh->InitParameters(filepath);
	mesh->SetName(name);

	m_pTargetScene->primitives.push_back(mesh);
	m_pTargetScene->m_preLoadList.push_back(mesh);

	UpdateDescriptorCount_CreateShape();
	return mesh;
}

shared_ptr<HSegment> HSceneManager::CreateSegment(string name, HFloat3 point1, HFloat3 point2)
{
	auto segment = make_shared<HSegment>(m_dxResources);
	segment->InitParameters(point1, point2);
	segment->SetName(name);

	m_pTargetScene->primitives.push_back(segment);
	m_pTargetScene->m_preLoadList.push_back(segment);

	UpdateDescriptorCount_CreateDebugLine();
	return segment;
}

shared_ptr<HTexture> HSceneManager::CreateTexture(string name, wstring texPath)
{
	auto pD3DDevice = m_dxResources->GetD3DDevice();

	shared_ptr<HTexture> pTexture = make_shared<HTexture>();
	pTexture->name = name;
	pTexture->filePath = texPath;
	DirectX::LoadWICTextureFromFile(pD3DDevice, pTexture->filePath.c_str(), pTexture->resource.GetAddressOf(), pTexture->mappedResource, pTexture->subresourceData);

	if (pTexture)
	{
		m_textureMap[pTexture->name] = pTexture;
		return pTexture;
	}
	return nullptr;
}

bool HSceneManager::BindMaterialToShape(shared_ptr<HShape> pShape, shared_ptr<HMaterial> pMaterial)
{
	if (!pShape || !pMaterial)
		return false;

	pShape->SetMaterial(pMaterial);
	if (pMaterial->TextureEnable())
		UpdateDescriptorCount_AssignTextureToShape();
	return true;
}

shared_ptr<HTexture> HSceneManager::GetTexture(string name)
{
	auto it = m_textureMap.find(name);
	bool texExist = it != m_textureMap.end();
	if (texExist)
		return it->second;
	return nullptr;
}

shared_ptr<Camera> HSceneManager::CreateCamera()
{
	auto camera = make_shared<Camera>(m_dxResources);
	m_pTargetScene->cameras.push_back(camera);
	camera->Init(70.0f, 0.01f, 1000.0f);
	camera->SetCameraBuffer();
	return camera;
}

shared_ptr<HPointLight> HSceneManager::CreatePointLight()
{
	auto pointLight = make_shared<HPointLight>();
	m_pTargetScene->lights.push_back(pointLight);
	return pointLight;
}

shared_ptr<HPBRMaterialMatte> HSceneManager::CreateMatteMaterial(const HFloat3& kd, const HFloat sigma)
{
	auto mat = make_shared<HPBRMaterialMatte>(kd, sigma);
	m_pTargetScene->pbrMaterials.push_back(mat);
	return mat;
}

shared_ptr<HPBRMaterialMirror> HSceneManager::CreateMirrorMaterial(const HFloat3 & kr)
{
	auto mat = make_shared<HPBRMaterialMirror>(kr);
	m_pTargetScene->pbrMaterials.push_back(mat);
	return mat;
}

shared_ptr<HPBRMaterialGlass> HSceneManager::CreateGlassMaterial(const HFloat3 & Kr, const HFloat3 & Kt, const HFloat eta)
{
	auto mat = make_shared<HPBRMaterialGlass>(Kr, Kt, eta);
	m_pTargetScene->pbrMaterials.push_back(mat);
	return mat;
}

shared_ptr<HScript> HSceneManager::CreateScript(const HSCRIPTTYPE scriptType, const shared_ptr<HObject>& pObject)
{
	switch (scriptType)
	{
	case HSCRIPTTYPE::HSCRIPT_TEST:
	{
		auto pScript = make_shared<HSTest>(pObject);
		m_pTargetScene->scripts.push_back(pScript);
		return pScript;
	}
	case HSCRIPTTYPE::HSCRIPT_FIRST_PERSONAL_CAMERA:
	{
		auto pScript = make_shared<HSFirstPersonalCamera>(pObject);
		m_pTargetScene->scripts.push_back(pScript);
		return pScript;
	}
	default:
		return nullptr;
	}
}

shared_ptr<HListener> HSceneManager::AddEventListener(const HEVENTTYPE eventType, const shared_ptr<HObject>& pObject, const function<void(HEventArg)>& pFunc)
{
	auto pListener = make_shared<HListener>(pObject, pFunc);
	switch (eventType)
	{
	case HEVENTTYPE::HEVENT_KEYDOWN:
	{
		HKeyDownEvent::GetInstance()->AddListener(pListener);
		break;
	}
	case HEVENTTYPE::HEVENT_KEYUP:
	{
		HKeyUpEvent::GetInstance()->AddListener(pListener);
		break;
	}
	case HEVENTTYPE::HEVENT_MOUSEDOWN:
	{
		HMouseDownEvent::GetInstance()->AddListener(pListener);
		break;
	}
	case HEVENTTYPE::HEVENT_MOUSEUP:
	{
		HMouseUpEvent::GetInstance()->AddListener(pListener);
		break;
	}
	case HEVENTTYPE::HEVENT_MOUSEMOVE:
	{
		HMouseMoveEvent::GetInstance()->AddListener(pListener);
		break;
	}
	default:
		return nullptr;
	}
	return pListener;
}

void HSceneManager::AddDescriptorCount(HUInt value)
{
	m_descriptorCount += value;
}

void HSceneManager::UpdateDescriptorCount_CreateShape()
{
	m_descriptorCount += 2;
}

void HSceneManager::UpdateDescriptorCount_CreateDebugLine()
{
	m_descriptorCount += 2;
}

void HSceneManager::UpdateDescriptorCount_AssignTextureToShape()
{
	m_descriptorCount += 1;
}
