#pragma once
#include "Box.h"
#include "HPointLight.h"
#include "HMatteMaterial.h"

class HScene
{
public:
	HScene();
	~HScene();

	void Init();
	void Update();
	void Render();

private:
	vector<Transform> m_transformNodes;
	vector<HMaterial> m_materials;
};
