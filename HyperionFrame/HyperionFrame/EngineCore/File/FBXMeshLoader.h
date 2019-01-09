#pragma once
#include "..\FBXImporter\Common\Common.h"
#include "HMesh.h"

namespace FBXMeshLoader
{
	void LoadContent(FbxScene* pScene);
	void LoadContent(FbxNode* pNode);
	void DisplayGeometricTransform(FbxNode* pNode);

	void LoadMesh(FbxNode* pNode);
	void LoadPolygons(FbxMesh* pMesh);

	void LoadTest();
}