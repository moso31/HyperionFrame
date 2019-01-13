#include "HLine.h"
#include "HMatteMaterial.h"
#include "HGlassMaterial.h"
#include "HMirrorMaterial.h"

HLine::HLine()
{
}

HLine::HLine(const shared_ptr<DXResource>& dxResources) :
	HPrimitive(dxResources)
{
	m_renderType = eRenderType::Line;
}

HLine::~HLine()
{
}
