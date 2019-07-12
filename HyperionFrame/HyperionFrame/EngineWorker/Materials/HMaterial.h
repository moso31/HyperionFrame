#pragma once
#include "header.h"
#include "ShaderStructures.h"

class HMaterial
{
public:
	HMaterial(string name);

	shared_ptr<HTexture> GetTexture();
	void SetTexture(shared_ptr<HTexture> pTexture);
	void ClearTexture();

	bool TextureEnable();

private:
	string m_name;

	CBufferMeshData m_color;
	shared_ptr<HTexture> m_texture;

	bool m_textureEnable;
};