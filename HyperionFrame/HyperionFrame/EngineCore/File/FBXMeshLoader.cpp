#include "FBXMeshLoader.h"
#include "HMesh.h"

namespace FBXMeshLoader
{
	void LoadContent(FbxScene* pScene, shared_ptr<HMesh>& pEngineMesh)
	{
		int i;
		FbxNode* lNode = pScene->GetRootNode();

		if (lNode)
		{
			for (i = 0; i < lNode->GetChildCount(); i++)
			{
				LoadContent(lNode->GetChild(i), pEngineMesh);
			}
		}
	}

	void LoadContent(FbxNode* pNode, shared_ptr<HMesh>& pEngineMesh)
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
				LoadMesh(pNode, pEngineMesh);
				break;

			case FbxNodeAttribute::eCamera:
				//DisplayCamera(pNode);
				break;

			case FbxNodeAttribute::eLight:
				//DisplayLight(pNode);
				break;
			}
		}

		DisplayGeometricTransform(pNode, pEngineMesh);

		for (i = 0; i < pNode->GetChildCount(); i++)
		{
			LoadContent(pNode->GetChild(i), pEngineMesh);
		}
	}

	void DisplayGeometricTransform(FbxNode * pNode, shared_ptr<HMesh>& pEngineMesh)
	{
		FbxVector4 lTmpVector;
		HFloat4 vec = HFloat4();

		//
		// Translation
		//
		lTmpVector = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
		vec = ReadHFLOAT4(lTmpVector);
		pEngineMesh->SetTranslation(vec.x, vec.y, vec.z);

		//
		// Rotation
		//
		lTmpVector = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
		vec = ReadHFLOAT4(lTmpVector);
		pEngineMesh->SetRotation(vec.x, vec.y, vec.z);

		//
		// Scaling
		//
		lTmpVector = pNode->GetGeometricScaling(FbxNode::eSourcePivot);
		vec = ReadHFLOAT4(lTmpVector);
		pEngineMesh->SetScale(vec.x, vec.y, vec.z);
	}

	void LoadMesh(FbxNode * pNode, shared_ptr<HMesh>& pEngineMesh)
	{
		pEngineMesh->SetName((char*)pNode->GetName());
		FbxMesh* lMesh = (FbxMesh*)pNode->GetNodeAttribute();

		LoadPolygons(lMesh, pEngineMesh);
	}

	void LoadPolygons(FbxMesh * pMesh, shared_ptr<HMesh>& pEngineMesh)
	{
		int i, j, lPolygonCount = pMesh->GetPolygonCount();
		FbxVector4* lControlPoints = pMesh->GetControlPoints();
		char header[100];

		//DisplayString("    Polygons");

		int vertexId = 0;
		for (i = 0; i < lPolygonCount; i++)
		{
			//DisplayInt("        Polygon ", i);
			int l;

			for (l = 0; l < pMesh->GetElementPolygonGroupCount(); l++)
			{
				FbxGeometryElementPolygonGroup* lePolgrp = pMesh->GetElementPolygonGroup(l);
				switch (lePolgrp->GetMappingMode())
				{
				case FbxGeometryElement::eByPolygon:
					if (lePolgrp->GetReferenceMode() == FbxGeometryElement::eIndex)
					{
						FBXSDK_sprintf(header, 100, "        Assigned to group: ");
						int polyGroupId = lePolgrp->GetIndexArray().GetAt(i);
						//DisplayInt(header, polyGroupId);
						break;
					}
				default:
					// any other mapping modes don't make sense
					//DisplayString("        \"unsupported group assignment\"");
					break;
				}
			}

			VertexPNT vertex;
			int lPolygonSize = pMesh->GetPolygonSize(i);

			for (j = 0; j < lPolygonSize; j++)
			{
				int lControlPointIndex = pMesh->GetPolygonVertex(i, j);
				vertex.pos = ReadHFLOAT3(lControlPoints[lControlPointIndex]);

				for (l = 0; l < pMesh->GetElementVertexColorCount(); l++)
				{
					FbxGeometryElementVertexColor* leVtxc = pMesh->GetElementVertexColor(l);
					FBXSDK_sprintf(header, 100, "            Color vertex: ");

					switch (leVtxc->GetMappingMode())
					{
					default:
						break;
					case FbxGeometryElement::eByControlPoint:
						switch (leVtxc->GetReferenceMode())
						{
						case FbxGeometryElement::eDirect:
							//DisplayColor(header, leVtxc->GetDirectArray().GetAt(lControlPointIndex));
							break;
						case FbxGeometryElement::eIndexToDirect:
						{
							int id = leVtxc->GetIndexArray().GetAt(lControlPointIndex);
							//DisplayColor(header, leVtxc->GetDirectArray().GetAt(id));
						}
						break;
						default:
							break; // other reference modes not shown here!
						}
						break;

					case FbxGeometryElement::eByPolygonVertex:
					{
						switch (leVtxc->GetReferenceMode())
						{
						case FbxGeometryElement::eDirect:
							//DisplayColor(header, leVtxc->GetDirectArray().GetAt(vertexId));
							break;
						case FbxGeometryElement::eIndexToDirect:
						{
							int id = leVtxc->GetIndexArray().GetAt(vertexId);
							//DisplayColor(header, leVtxc->GetDirectArray().GetAt(id));
						}
						break;
						default:
							break; // other reference modes not shown here!
						}
					}
					break;

					case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
					case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
					case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
						break;
					}
				}
				for (l = 0; l < pMesh->GetElementUVCount(); ++l)
				{
					FbxGeometryElementUV* leUV = pMesh->GetElementUV(l);
					FBXSDK_sprintf(header, 100, "            Texture UV: ");

					switch (leUV->GetMappingMode())
					{
					default:
						break;
					case FbxGeometryElement::eByControlPoint:
						switch (leUV->GetReferenceMode())
						{
						case FbxGeometryElement::eDirect:
							vertex.uv = ReadHFLOAT2(leUV->GetDirectArray().GetAt(lControlPointIndex));
							break;
						case FbxGeometryElement::eIndexToDirect:
						{
							int id = leUV->GetIndexArray().GetAt(lControlPointIndex);
							vertex.uv = ReadHFLOAT2(leUV->GetDirectArray().GetAt(id));
						}
						break;
						default:
							break; // other reference modes not shown here!
						}
						break;

					case FbxGeometryElement::eByPolygonVertex:
					{
						int lTextureUVIndex = pMesh->GetTextureUVIndex(i, j);
						switch (leUV->GetReferenceMode())
						{
						case FbxGeometryElement::eDirect:
						case FbxGeometryElement::eIndexToDirect:
						{
							vertex.uv = ReadHFLOAT2(leUV->GetDirectArray().GetAt(lTextureUVIndex));
						}
						break;
						default:
							break; // other reference modes not shown here!
						}
					}
					break;

					case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
					case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
					case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
						break;
					}
				}
				for (l = 0; l < pMesh->GetElementNormalCount(); ++l)
				{
					FbxGeometryElementNormal* leNormal = pMesh->GetElementNormal(l);
					FBXSDK_sprintf(header, 100, "            Normal: ");

					if (leNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
					{
						switch (leNormal->GetReferenceMode())
						{
						case FbxGeometryElement::eDirect:
							vertex.norm = ReadHFLOAT3(leNormal->GetDirectArray().GetAt(vertexId));
							break;
						case FbxGeometryElement::eIndexToDirect:
						{
							int id = leNormal->GetIndexArray().GetAt(vertexId);
							vertex.norm = ReadHFLOAT3(leNormal->GetDirectArray().GetAt(id));
						}
						break;
						default:
							break; // other reference modes not shown here!
						}
					}

				}
				for (l = 0; l < pMesh->GetElementTangentCount(); ++l)
				{
					FbxGeometryElementTangent* leTangent = pMesh->GetElementTangent(l);
					FBXSDK_sprintf(header, 100, "            Tangent: ");

					if (leTangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
					{
						switch (leTangent->GetReferenceMode())
						{
						case FbxGeometryElement::eDirect:
							//Display3DVector(header, leTangent->GetDirectArray().GetAt(vertexId));
							break;
						case FbxGeometryElement::eIndexToDirect:
						{
							int id = leTangent->GetIndexArray().GetAt(vertexId);
							//Display3DVector(header, leTangent->GetDirectArray().GetAt(id));
						}
						break;
						default:
							break; // other reference modes not shown here!
						}
					}

				}
				for (l = 0; l < pMesh->GetElementBinormalCount(); ++l)
				{

					FbxGeometryElementBinormal* leBinormal = pMesh->GetElementBinormal(l);

					FBXSDK_sprintf(header, 100, "            Binormal: ");
					if (leBinormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
					{
						switch (leBinormal->GetReferenceMode())
						{
						case FbxGeometryElement::eDirect:
							//Display3DVector(header, leBinormal->GetDirectArray().GetAt(vertexId));
							break;
						case FbxGeometryElement::eIndexToDirect:
						{
							int id = leBinormal->GetIndexArray().GetAt(vertexId);
							//Display3DVector(header, leBinormal->GetDirectArray().GetAt(id));
						}
						break;
						default:
							break; // other reference modes not shown here!
						}
					}
				}
				vertexId++;

				pEngineMesh->AddVertex(vertex);
				int x = i * 3 + j;
				int meshIdx = i * 3 + j;
				if (j % 3 == 1) meshIdx++;
				else if (j % 3 == 2) meshIdx--;
				pEngineMesh->AddIndex(meshIdx);
			} // for polygonSize
		} // for polygonCount

		//check visibility for the edges of the mesh
		for (int l = 0; l < pMesh->GetElementVisibilityCount(); ++l)
		{
			FbxGeometryElementVisibility* leVisibility = pMesh->GetElementVisibility(l);
			//FBXSDK_sprintf(header, 100, "    Edge Visibility : ");
			//DisplayString(header);
			switch (leVisibility->GetMappingMode())
			{
			default:
				break;
				//should be eByEdge
			case FbxGeometryElement::eByEdge:
				//should be eDirect
				for (j = 0; j != pMesh->GetMeshEdgeCount(); ++j)
				{
					//DisplayInt("        Edge ", j);
					//DisplayBool("              Edge visibility: ", leVisibility->GetDirectArray().GetAt(j));
				}

				break;
			}
		}
		//DisplayString("");
	}

	void LoadFBXFile(string filepath, shared_ptr<HMesh>& pEngineMesh)
	{
		FbxManager* lSdkManager = NULL;
		FbxScene* lScene = NULL;
		bool lResult = false;

		// Prepare the FBX SDK.
		InitializeSdkObjects(lSdkManager, lScene);
		// Load the scene.

		// The example can take a FBX file as an argument.
		FbxString lFilePath(filepath.data());

		if (lFilePath.IsEmpty())
			return;

		lResult = LoadScene(lSdkManager, lScene, lFilePath.Buffer());
		if (!lResult)
			return;

		FbxNode* lNode = lScene->GetRootNode();

		if (lNode)
		{
			for (int i = 0; i < lNode->GetChildCount(); i++)
			{
				LoadContent(lNode->GetChild(i), pEngineMesh);
			}
		}
	}
}