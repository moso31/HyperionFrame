#include "HSceneManager.h"
#include "Box.h"
#include "Sphere.h"

bool ShapeCharacter::IsSame(const ShapeCharacter &other) const
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

HSceneManager::HSceneManager(std::shared_ptr<DXResource> dxResources, ComPtr<ID3D12GraphicsCommandList> pCommandList) :
	m_dxResources(dxResources),
	m_pCommandList(pCommandList)
{
}

HSceneManager::~HSceneManager()
{
}

Box * HSceneManager::CreateBox(float width, float height, float depth)
{
	auto box = new Box(m_dxResources);

	ShapeCharacter sc;
	sc.type = eShapeType::HSHAPE_BOX;
	for (int i = 0; i < m_characterTable.size(); i++)
	{
		ShapeCharacter ch = m_characterTable[i];
		if (ch.IsSame(sc))
		{
			box->InitParameters();
			box->SetShapeBuffer(&ch.bufferData);
			m_shapeMapping.push_back(i);
			return box;
		}
	}

	box->InitParameters();
	box->GenerateShapeBuffer(m_pCommandList, &sc.bufferData);
	m_shapeMapping.push_back((int)m_characterTable.size());
	m_characterTable.push_back(sc);
	return box;
}

Sphere * HSceneManager::CreateSphere(float radius, int segmentHorizontal, int segmentVertical)
{
	auto sphere = new Sphere(m_dxResources);

	ShapeCharacter sc;
	sc.type = eShapeType::HSHAPE_SPHERE;
	for (int i = 0; i < m_characterTable.size(); i++)
	{
		ShapeCharacter ch = m_characterTable[i];
		if (ch.IsSame(sc))
		{
			sphere->InitParameters(radius, segmentHorizontal, segmentVertical);
			sphere->SetShapeBuffer(&ch.bufferData);
			m_shapeMapping.push_back(i);
			return sphere;
		}
	}

	sphere->InitParameters(radius, segmentHorizontal, segmentVertical);
	sphere->GenerateShapeBuffer(m_pCommandList, &sc.bufferData);
	m_shapeMapping.push_back((int)m_characterTable.size());
	m_characterTable.push_back(sc);
	return sphere;
}

int HSceneManager::GetShapeCharacterCount() 
{
	return (int)m_characterTable.size(); 
}

int HSceneManager::GetShapeBufferIndex(size_t shapeIndex) 
{
	return m_shapeMapping[shapeIndex]; 
}
