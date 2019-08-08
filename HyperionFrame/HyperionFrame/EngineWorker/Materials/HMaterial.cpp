#include "HMaterial.h"
#include "HTexture.h"

HMaterial::HMaterial(string name) :
	m_name(name),
	m_texture(nullptr),
	m_textureEnable(false)
{
	m_color.vertColor = 0.0f;
}

shared_ptr<HTexture> HMaterial::GetTexture()
{
	return m_texture;
}

void HMaterial::SetTexture(shared_ptr<HTexture> pTexture)
{
	if (pTexture)
	{
		m_texture = pTexture;
		m_textureEnable = true;
	}
}

void HMaterial::ClearTexture()
{
	m_texture = nullptr;
}

bool HMaterial::TextureEnable()
{
	return m_textureEnable;
}