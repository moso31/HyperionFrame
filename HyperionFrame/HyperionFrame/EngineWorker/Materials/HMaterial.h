#pragma once
#include "ShaderStructures.h"

class HMaterial
{
public:
	HMaterial() = default;

	void Init();

private:
	CBufferMeshData m_color;
	// HTexture m_texture;
};