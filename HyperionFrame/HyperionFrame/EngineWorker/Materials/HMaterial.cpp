#include "HMaterial.h"

HMaterial::HMaterial(string name) :
	m_name(name),
	m_textureName(""),
	m_textureEnable(false)
{
	m_color.vertColor = 0.0f;
}

void HMaterial::SetTexture(string textureName)
{
	m_textureName = textureName.c_str();
	m_textureEnable = true;
}
