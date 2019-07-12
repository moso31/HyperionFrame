#include "HDescriptorManager.h"
#include "DXResource.h"

HDescriptorManager::HDescriptorManager(const std::shared_ptr<DXResource>& dxResource) :
	m_shapeCount(0),
	m_shapeTextureCount(0),
	m_debugMsgLineCount(0),
	m_worldViewProjCount(0)
{
}

void HDescriptorManager::Init()
{
	m_worldViewProjCount = 1;
}

void HDescriptorManager::Trace(eDescriptorTraceType type)
{
	switch (type)
	{
	case DTT_CREATESHAPE:
		m_shapeCount += 2;
		break;
	case DTT_CREATEDEBUGMSGLINE:
		m_debugMsgLineCount += 2;
		break;
	case DTT_BINDMATERIALTOSHAPE:
		m_shapeTextureCount++;
		break;
	default:
		break;
	}
}

HUInt HDescriptorManager::GetDescriptorCount()
{
	return DXResource::c_frameCount * (m_shapeCount + m_shapeTextureCount + m_debugMsgLineCount) + m_worldViewProjCount;
}
