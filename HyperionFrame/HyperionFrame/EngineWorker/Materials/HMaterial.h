#pragma once
#include <string>
#include "ShaderStructures.h"

using namespace std;

class HMaterial
{
public:
	HMaterial() = default;

	string GetTextureName() { return m_textureName; }

	// ºÏ≤‚ «∑Ò”–Œ∆¿Ì°£
	bool TextureEnable() { return m_textureEnable; }

	void SetTexture(string textureName);

private:
	CBufferMeshData m_color;

	bool m_textureEnable;
	string m_textureName;
};