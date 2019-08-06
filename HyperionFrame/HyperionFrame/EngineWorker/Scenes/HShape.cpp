#include "HShape.h"
#include "DirectXHelper.h"

#include "HPBRMaterialMatte.h"
#include "HPBRMaterialGlass.h"
#include "HPBRMaterialMirror.h"

HShape::HShape()
{
}

HShape::HShape(const shared_ptr<DXResource>& dxResources) :
	HPrimitive(dxResources)
{
}

HShape::~HShape()
{
}

HUInt HShape::GetFaceCount()
{
	return (HUInt)m_indices.size() / 3;
}

Triangle HShape::GetFace(HUInt faceIndex)
{
	HInt index = faceIndex * 3;
	Triangle tri;
	tri.p[0] = m_vertices[m_indices[index]].pos;
	tri.p[1] = m_vertices[m_indices[index + 1]].pos;
	tri.p[2] = m_vertices[m_indices[index + 2]].pos;
	return tri;
}

TriangleUV HShape::GetUVs(HUInt faceIndex)
{
	HInt index = faceIndex * 3;
	TriangleUV triUV;
	triUV.p[0] = m_vertices[m_indices[index]].uv;
	triUV.p[1] = m_vertices[m_indices[index + 1]].uv;
	triUV.p[2] = m_vertices[m_indices[index + 2]].uv;
	return triUV;
}

void HShape::SetMaterial(shared_ptr<HMaterial> mat)
{
	m_material = mat;
}

void HShape::SetPBRMaterial(shared_ptr<HPBRMaterial> mat)
{
	m_materialPBR = mat;
	
	// 这里临时使用PBR材质的漫反射作为材质的实时显示颜色。
	m_cbMeshData.vertColor = mat->GetDiffuse();
}

HUInt HShape::GetAlignedConstantBufferSize()
{
	return ((sizeof(ModelViewProjectionConstantBuffer) + 255) + (sizeof(CBufferMeshData) + 255)) & ~255;
}
