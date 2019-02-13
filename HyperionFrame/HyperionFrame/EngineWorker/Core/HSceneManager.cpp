#include "HSceneManager.h"
#include "DirectXHelper.h"

#include "Box.h"
#include "Sphere.h"
#include "HMesh.h"
#include "HSegment.h"

#include "HScriptType.h"
#include "HSTest.h"

HSceneManager::HSceneManager()
{
}

HSceneManager::HSceneManager(std::shared_ptr<DXResource> dxResources, const ComPtr<ID3D12DescriptorHeap>& pCbvHeap, const ComPtr<ID3D12GraphicsCommandList>& pCommandList) :
	m_dxResources(dxResources),
	m_pCommandList(pCommandList)
{
}

HSceneManager::~HSceneManager()
{
}

shared_ptr<Box> HSceneManager::CreateBox(float width, float height, float depth)
{
	auto box = make_shared<Box>(m_dxResources);
	box->InitParameters();
	//box->GeneratePrimitiveBuffer(m_pCommandList);
	return box;
}

shared_ptr<Sphere> HSceneManager::CreateSphere(float radius, int segmentHorizontal, int segmentVertical)
{
	auto sphere = make_shared<Sphere>(m_dxResources);
	sphere->InitParameters(radius, segmentHorizontal, segmentVertical);
	//sphere->GeneratePrimitiveBuffer(m_pCommandList);
	return sphere;
}

shared_ptr<HMesh> HSceneManager::CreateMesh(string filepath)
{
	auto mesh = make_shared<HMesh>(m_dxResources);
	mesh->InitParameters(filepath);
	//mesh->GeneratePrimitiveBuffer(m_pCommandList);
	return mesh;
}

shared_ptr<HSegment> HSceneManager::CreateSegment(XMFLOAT3 point1, XMFLOAT3 point2)
{
	auto segment = make_shared<HSegment>(m_dxResources);
	segment->InitParameters(point1, point2);
	//segment->GeneratePrimitiveBuffer(m_pCommandList);
	return segment;
}

shared_ptr<HScript> HSceneManager::CreateScript(const HSCRIPTTYPE scriptType, const shared_ptr<HObject>& pObject)
{
	switch (scriptType)
	{
	case HSCRIPTTYPE::HSCRIPT_TEST:
	{
		auto pScript = make_shared<HSTest>(pObject);
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
	case HEVENTTYPE::HEVENT_MOUSEDOWN:
	{
		HMouseDownEvent::GetInstance()->AddListener(pListener);
		break;
	}
	default:
		return nullptr;
	}
	return pListener;
}
