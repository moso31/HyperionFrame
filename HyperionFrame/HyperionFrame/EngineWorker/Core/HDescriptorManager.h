#pragma once
#include "header.h"

enum eDescriptorTraceType
{
	DTT_NONE,
	DTT_CREATESHAPE,
	DTT_CREATEDEBUGMSGLINE,
	DTT_BINDMATERIALTOSHAPE,
};

class HDescriptorManager
{
public:
	HDescriptorManager() = default;
	HDescriptorManager(const std::shared_ptr<DXResource>& dxResource);

	void Init();
	void Trace(eDescriptorTraceType type);
	HUInt GetDescriptorCount();

private:
	shared_ptr<DXResource>		m_dxResources;

	HUInt	m_shapeCount;
	HUInt	m_shapeTextureCount;
	HUInt	m_debugMsgLineCount;
	HUInt	m_worldViewProjCount;
};