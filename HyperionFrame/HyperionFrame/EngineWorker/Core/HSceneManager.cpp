#include "HSceneManager.h"
#include "DirectXHelper.h"

#include "Box.h"
#include "Sphere.h"
#include "HMesh.h"
#include "HSegment.h"

bool CommonFeatureParams::IsSame(const CommonFeatureParams &other) const
{
	if (type != other.type)
		return false;

	if (params.size() != other.params.size())
		return false;

	for (int i = 0; i < params.size(); i++)
	{
		if (params[i] != other.params[i])
			return false;
	}

	return true;
}

HSceneManager::HSceneManager()
{
}

HSceneManager::HSceneManager(std::shared_ptr<DXResource> dxResources, const ComPtr<ID3D12DescriptorHeap>& pCbvHeap, const ComPtr<ID3D12GraphicsCommandList>& pCommandList) :
	m_dxResources(dxResources),
	m_cbvHeap(pCbvHeap),
	m_pCommandList(pCommandList)
{
}

HSceneManager::~HSceneManager()
{
}

shared_ptr<Box> HSceneManager::CreateBox(float width, float height, float depth)
{
	auto box = make_shared<Box>(m_dxResources);

	CommonFeatureParams sc;
	sc.type = ePrimitiveType::BOX;

	// �����������������Ѿ�������������ƥ�䣬�ͺ��乲�û��档
	for (int i = 0; i < m_commonFeatureTable.size(); i++)
	{
		CommonFeatureParams ch = m_commonFeatureTable[i];
		if (ch.IsSame(sc))
		{
			box->InitParameters();
			box->SetPrimitiveBuffer(&ch.bufferData);
			return box;
		}
	}

	// ���򣬽��������������������С�
	box->InitParameters();
	box->GeneratePrimitiveBuffer(m_pCommandList, &sc.bufferData);
	m_commonFeatureTable.push_back(sc);
	return box;
}

shared_ptr<Sphere> HSceneManager::CreateSphere(float radius, int segmentHorizontal, int segmentVertical)
{
	auto sphere = make_shared<Sphere>(m_dxResources);

	CommonFeatureParams sc;
	sc.type = ePrimitiveType::SPHERE;

	// �����������������Ѿ�������������ƥ�䣬�ͺ��乲�û��档
	for (int i = 0; i < m_commonFeatureTable.size(); i++)
	{
		CommonFeatureParams ch = m_commonFeatureTable[i];
		if (ch.IsSame(sc))
		{
			sphere->InitParameters(radius, segmentHorizontal, segmentVertical);
			sphere->SetPrimitiveBuffer(&ch.bufferData);
			return sphere;
		}
	}

	// ���򣬽��������������������С�
	sphere->InitParameters(radius, segmentHorizontal, segmentVertical);
	sphere->GeneratePrimitiveBuffer(m_pCommandList, &sc.bufferData);
	m_commonFeatureTable.push_back(sc);
	return sphere;
}

shared_ptr<HMesh> HSceneManager::CreateMesh(string filepath)
{
	auto mesh = make_shared<HMesh>(m_dxResources);

	CommonFeatureParams sc;
	sc.type = ePrimitiveType::MESH;

	// �����������������Ѿ�������������ƥ�䣬�ͺ��乲�û��档
	for (int i = 0; i < m_commonFeatureTable.size(); i++)
	{
		CommonFeatureParams ch = m_commonFeatureTable[i];
		if (ch.IsSame(sc))
		{
			mesh->InitParameters(filepath);
			mesh->SetPrimitiveBuffer(&ch.bufferData);
			return mesh;
		}
	}

	// ���򣬽��������������������С�
	mesh->InitParameters(filepath);
	mesh->GeneratePrimitiveBuffer(m_pCommandList, &sc.bufferData);
	m_commonFeatureTable.push_back(sc);
	return mesh;
}

shared_ptr<HSegment> HSceneManager::CreateSegment(XMFLOAT3 point1, XMFLOAT3 point2)
{
	auto segment = make_shared<HSegment>(m_dxResources);

	CommonFeatureParams sc;
	sc.type = ePrimitiveType::SEGMENT;

	// �����������������Ѿ�������������ƥ�䣬�ͺ��乲�û��档
	for (int i = 0; i < m_commonFeatureTable.size(); i++)
	{
		CommonFeatureParams ch = m_commonFeatureTable[i];
		if (ch.IsSame(sc))
		{
			segment->InitParameters(point1, point2);
			segment->SetPrimitiveBuffer(&ch.bufferData);
			return segment;
		}
	}

	// ���򣬽��������������������С�
	segment->InitParameters(point1, point2);
	segment->GeneratePrimitiveBuffer(m_pCommandList, &sc.bufferData);
	m_commonFeatureTable.push_back(sc);
	return segment;
}

int HSceneManager::GetCommonFeatureTableCount() 
{
	return (int)m_commonFeatureTable.size(); 
}
