#pragma once
#include <string>
#include "ShaderStructures.h"

using namespace std;

class HMaterial
{
public:
	HMaterial(string name);

	string GetTextureName() { return m_textureName; }

	// ºÏ≤‚ «∑Ò”–Œ∆¿Ì°£
	bool TextureEnable() { return m_textureEnable; }

	void SetTexture(string textureName);

private:
	string m_name;

	CBufferMeshData m_color;

	bool m_textureEnable;
	string m_textureName;
};