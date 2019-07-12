#include "HLine.h"
#include "DirectXHelper.h"

#include "HPBRMaterialMatte.h"
#include "HPBRMaterialGlass.h"
#include "HPBRMaterialMirror.h"

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

HUInt HLine::GetAlignedConstantBufferSize()
{
	return ((sizeof(ModelViewProjectionConstantBuffer) + 255) + (sizeof(CBufferMeshData) + 255)) & ~255;
}
