#pragma once
#include <memory>
#include <string>

#include "FBXDataConvert.h"
#include "..\FBXImporter\Common\Common.h"

using namespace std;

class HMesh;

namespace FBXMeshLoader
{
	void LoadContent(FbxScene* pScene, shared_ptr<HMesh>& pEngineMesh);
	void LoadContent(FbxNode* pNode, shared_ptr<HMesh>& pEngineMesh);
	void DisplayGeometricTransform(FbxNode* pNode, shared_ptr<HMesh>& pEngineMesh);

	void LoadMesh(FbxNode* pNode, shared_ptr<HMesh>& pEngineMesh);
	void LoadPolygons(FbxMesh* pMesh, shared_ptr<HMesh>& pEngineMesh);

	void LoadFBXFile(string filepath, shared_ptr<HMesh>& pEngineMesh);
}