#include "FBXMeshLoader.h"

namespace FBXMeshLoader
{
	void LoadContent(FbxScene* pScene)
	{
		int i;
		FbxNode* lNode = pScene->GetRootNode();

		if (lNode)
		{
			for (i = 0; i < lNode->GetChildCount(); i++)
			{
				LoadContent(lNode->GetChild(i));
			}
		}
	}

	void LoadContent(FbxNode* pNode)
	{
		FbxNodeAttribute::EType lAttributeType;
		int i;

		if (pNode->GetNodeAttribute() == NULL)
		{
			FBXSDK_printf("NULL Node Attribute\n\n");
		}
		else
		{
			lAttributeType = (pNode->GetNodeAttribute()->GetAttributeType());

			switch (lAttributeType)
			{
			default:
				break;

			case FbxNodeAttribute::eMesh:
				LoadMesh(pNode);
				break;

			case FbxNodeAttribute::eCamera:
				//DisplayCamera(pNode);
				break;

			case FbxNodeAttribute::eLight:
				//DisplayLight(pNode);
				break;
			}
		}

		DisplayGeometricTransform(pNode);

		for (i = 0; i < pNode->GetChildCount(); i++)
		{
			LoadContent(pNode->GetChild(i));
		}
	}

	void DisplayGeometricTransform(FbxNode * pNode)
	{
		FbxVector4 lTmpVector;

		FBXSDK_printf("    Geometric Transformations\n");

		//
		// Translation
		//
		lTmpVector = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
		FBXSDK_printf("        Translation: %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);

		//
		// Rotation
		//
		lTmpVector = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
		FBXSDK_printf("        Rotation:    %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);

		//
		// Scaling
		//
		lTmpVector = pNode->GetGeometricScaling(FbxNode::eSourcePivot);
		FBXSDK_printf("        Scaling:     %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);
	}

	void LoadMesh(FbxNode * pNode)
	{
		FbxMesh* lMesh = (FbxMesh*)pNode->GetNodeAttribute();

		//DisplayString("Mesh Name: ", (char *)pNode->GetName());
		LoadPolygons(lMesh);
	}

	void LoadPolygons(FbxMesh * pMesh)
	{
	}

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

		if (lFilePath.IsEmpty())
			return;

		if (!lResult)
			return;

		FbxNode* lNode = lScene->GetRootNode();

		if (lNode)
		{
			for (int i = 0; i < lNode->GetChildCount(); i++)
			{
				LoadContent(lNode->GetChild(i));
			}
		}
	}
}