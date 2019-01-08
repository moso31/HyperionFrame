#include "FBXMeshLoader.h"
#include "..\FBXImporter\Common\Common.h"

namespace FBXMeshLoader
{
	void LoadTest()
	{
		FbxManager* lSdkManager = NULL;
		FbxScene* lScene = NULL;
		bool lResult;

		// Prepare the FBX SDK.
		InitializeSdkObjects(lSdkManager, lScene);
		// Load the scene.

		// The example can take a FBX file as an argument.
		FbxString lFilePath("D:\\test.fbx");
	}
}