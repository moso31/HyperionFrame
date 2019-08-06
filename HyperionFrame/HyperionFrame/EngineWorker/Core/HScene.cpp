#include "HScene.h"
#include "DirectXHelper.h"
#include "DDSTextureLoader12.h"
#include "WICTextureLoader12.h"

#include "whitted.h"
#include "HDefaultSampler.h"

#include "Box.h"
#include "Sphere.h"
#include "HMesh.h"
#include "HSegment.h"
#include "HPointLight.h"
#include "HPBRMaterialMatte.h"
#include "HPBRMaterialGlass.h"
#include "HPBRMaterialMirror.h"
#include "HMaterial.h"
#include "HTexture.h"

#include "HInput.h"

#include "HSTest.h"
#include "HSFirstPersonalCamera.h"
#define BindScript(classType, scriptType, pObject) dynamic_pointer_cast<classType>(m_sceneManager->CreateScript(scriptType, pObject))
#define RegisterEventListener(object, script, eventType, pFunction) m_sceneManager->AddEventListener(eventType, object, std::bind(&pFunction, script, std::placeholders::_1));

HScene::HScene(const std::shared_ptr<DXResource>& dxResource) :
	m_dxResources(dxResource),
	m_bvhTree(nullptr),
	m_cbvDescriptorSize(0),
	m_makingProcessIndex(0)
{
}

HScene::~HScene()
{
}

void HScene::OnResize()
{
	m_mainCamera->OnResize();
}

void HScene::Init(ComPtr<ID3D12GraphicsCommandList> pCommandList)
{
	InitRendererData(pCommandList);
	InitTextureData(pCommandList);

	RegisterEventListener(shared_from_this(), shared_from_this(), HEVENTTYPE::HEVENT_KEYDOWN, HScene::OnKeyDown);
	RegisterEventListener(shared_from_this(), shared_from_this(), HEVENTTYPE::HEVENT_MOUSEDOWN, HScene::OnMouseDown);

	InitPrimitiveData();
	InitStructureData();
	InitSamplers();
}

void HScene::InitRendererData(ComPtr<ID3D12GraphicsCommandList> pCommandList)
{
	m_sceneManager = make_shared<HSceneManager>(m_dxResources, shared_from_this());

	auto pD3DDevice = m_dxResources->GetD3DDevice();
	
	m_cbvDescriptorSize = pD3DDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.NumDescriptors = DXResource::c_frameCount * 0 + 1;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	// 此标志指示此描述符堆可以绑定到管道，并且其中包含的描述符可以由根表引用。
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	DX::ThrowIfFailed(pD3DDevice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&m_cbvSrvHeap)));
	DX::NAME_D3D12_OBJECT(m_cbvSrvHeap);

	D3D12_DESCRIPTOR_HEAP_DESC samplerHeapDesc = {};
	samplerHeapDesc.NumDescriptors = 1;
	samplerHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
	samplerHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	DX::ThrowIfFailed(pD3DDevice->CreateDescriptorHeap(&samplerHeapDesc, IID_PPV_ARGS(&m_samplerHeap)));
	DX::NAME_D3D12_OBJECT(m_samplerHeap);
}

void HScene::InitTextureData(ComPtr<ID3D12GraphicsCommandList> pCommandList)
{
	auto pD3DDevice = m_dxResources->GetD3DDevice();

	shared_ptr<HTexture> pTexture = make_shared<HTexture>();
	pTexture->name = "firstTex";
	pTexture->filePath = L"E:\\1.bmp";
	DirectX::LoadWICTextureFromFile(pD3DDevice, pTexture->filePath.c_str(), pTexture->resource.GetAddressOf(), pTexture->mappedResource, pTexture->subresourceData);
	
	m_textureMap[pTexture->name] = pTexture;
}

void HScene::InitPrimitiveData()
{
	m_mainCamera = m_sceneManager->CreateCamera();
	m_mainCamera->SetTranslation(9.0f, 6.0f, 4.0f);
	m_mainCamera->SetLookAt(0.0f, 0.0f, 0.0f);

	shared_ptr<HSFirstPersonalCamera> pScript_first_personal_camera = BindScript(HSFirstPersonalCamera, HSCRIPT_FIRST_PERSONAL_CAMERA, m_mainCamera);

	RegisterEventListener(m_mainCamera, pScript_first_personal_camera, HEVENTTYPE::HEVENT_KEYDOWN, HSFirstPersonalCamera::OnKeyDown);
	RegisterEventListener(m_mainCamera, pScript_first_personal_camera, HEVENTTYPE::HEVENT_KEYUP, HSFirstPersonalCamera::OnKeyUp);
	RegisterEventListener(m_mainCamera, pScript_first_personal_camera, HEVENTTYPE::HEVENT_MOUSEMOVE, HSFirstPersonalCamera::OnMouseMove);

	HFloat3 red = { 1.0f, 0.0f, 0.0f },
		green = { 0.0f, 1.0f, 0.0f },
		blue = { 0.0f, 0.0f, 1.0f },
		yellow = { 1.0f, 1.0f, 0.0f },
		white = { 1.0f, 1.0f, 1.0f },
		mirror_white = { 0.9f, 0.9f, 0.9f },
		gray = { 0.8f, 0.8f, 0.8f };
	HFloat sig = 90.0f;
	shared_ptr<HPBRMaterial> mtrl[7] = {
		m_sceneManager->CreateMatteMaterial(green, sig),
		m_sceneManager->CreateMatteMaterial(red, sig),
		m_sceneManager->CreateMatteMaterial(blue, sig),
		m_sceneManager->CreateMatteMaterial(yellow, sig),
		m_sceneManager->CreateGlassMaterial(white, white, 1.55f),
		m_sceneManager->CreateMatteMaterial(gray, sig),
		m_sceneManager->CreateMirrorMaterial(mirror_white),
	};

	shared_ptr<HSTest> pScript;
	
	shared_ptr<HShape> pShape;
	shared_ptr<HLine> pLine;

	pShape = m_sceneManager->CreateBox("wall y+");
	pShape->SetTranslation(0.0f, 10.6f, 0.0f);
	pShape->SetPBRMaterial(mtrl[1]);
	pShape->SetScale(20.0f, 1.0f, 20.0f);

	pShape = m_sceneManager->CreateBox("wall x-");
	pShape->SetTranslation(-10.0f, 0.0f, 0.0f);
	pShape->SetPBRMaterial(mtrl[2]);
	pShape->SetScale(1.0f, 20.0f, 20.0f);

	pShape = m_sceneManager->CreateBox("wall x+");
	pShape->SetTranslation(+10.0f, 0.0f, 0.0f);
	pShape->SetPBRMaterial(mtrl[0]);
	pShape->SetScale(1.0f, 20.0f, 20.0f);

	pShape = m_sceneManager->CreateBox("wall z-");
	pShape->SetTranslation(0.0f, 0.0f, -10.0f);
	pShape->SetPBRMaterial(mtrl[5]);
	pShape->SetScale(20.0f, 20.0f, 1.0f);

	pShape = m_sceneManager->CreateBox("wall z+");
	pShape->SetTranslation(0.0f, 0.0f, +10.0f);
	pShape->SetPBRMaterial(mtrl[5]);
	pShape->SetScale(20.0f, 20.0f, 1.0f);

	pShape = m_sceneManager->CreateMesh("MayaFBXObject", "D:\\test.fbx");
	pShape->SetPBRMaterial(mtrl[6]);
	pShape->SetTranslation(-3.0f, 2.5f, -4.0f);
	pShape->SetScale(5.0f, 5.0f, 5.0f);
	pShape->SetRotation(0.0f, 0.3f, 0.0f);
	//pScript = BindScript(HSTest, HSCRIPTTYPE::HSCRIPT_TEST, pShape);

	//pShape = m_sceneManager->CreateBox("box big");
	//pShape->SetTranslation(-3.0f, 2.5f, -4.0f);
	//pShape->SetScale(5.0f, 5.0f, 5.0f);
	//pShape->SetRotation(0.0f, -0.3f, 0.0f);
	//pShape->SetMaterial(mtrl[4]);

	pShape = m_sceneManager->CreateSphere("sphere", 1.0f, 64, 64);
	pShape->SetTranslation(1.5f, 2.0f, 0.0f);
	pShape->SetScale(2.0f, 2.0f, 2.0f);
	pShape->SetPBRMaterial(mtrl[6]);

	pShape = m_sceneManager->CreateBox("box small");
	pShape->SetTranslation(5.0f, 1.0f, -2.0f);
	pShape->SetScale(2.0f, 2.0f, 2.0f);
	pShape->SetPBRMaterial(mtrl[4]);
	pShape->GetMaterial()->LoadTexture("firstTexture");

	//int iLineCount = 20;
	//for (HInt i = -iLineCount; i <= iLineCount; i++)
	//{
	//	HFloat pi = (HFloat)i;
	//	HFloat pLineCount = (HFloat)iLineCount;
	//	pLine = m_sceneManager->CreateSegment("grid", HFloat3(pi, 0.0f, -pLineCount), HFloat3(pi, 0.0f, pLineCount));
	//	pLine = m_sceneManager->CreateSegment("grid", HFloat3(-pLineCount, 0.0f, pi), HFloat3(pLineCount, 0.0f, pi));
	//}

	HInt chessSize = 9;
	for (HInt i = -chessSize; i <= chessSize; i++)
	{
		for (HInt j = -chessSize; j <= chessSize; j++)
		{
			pShape = m_sceneManager->CreateBox("chess boxes");
			if ((i + j) % 2)
			{
				pShape->SetTranslation(-i, -0.7f, j);
				pShape->SetPBRMaterial(mtrl[1]);
			}
			else
			{
				pShape->SetTranslation(-i, -0.5f, j);
				pShape->SetPBRMaterial(mtrl[3]);
			}
		}
	}

	auto pointLight = m_sceneManager->CreatePointLight();
	HFloat3 lightPos = { 0.0f, 10.0f, 5.0f };
	pointLight->SetTranslation(lightPos.x, lightPos.y, lightPos.z);
	HFloat brightness = 100.0f;
	pointLight->SetIntensity(brightness, brightness, brightness);

	pointLight = m_sceneManager->CreatePointLight();
	pointLight->SetTranslation(-lightPos.x, lightPos.y, -lightPos.z);
	brightness = 100.0f;
	pointLight->SetIntensity(brightness, brightness, brightness);

	pointLight = m_sceneManager->CreatePointLight();
	pointLight->SetTranslation(lightPos.z, lightPos.y, lightPos.x);
	brightness = 100.0f;
	pointLight->SetIntensity(brightness, brightness, brightness);

	pointLight = m_sceneManager->CreatePointLight();
	pointLight->SetTranslation(-lightPos.z, lightPos.y, -lightPos.x);
	brightness = 100.0f;
	pointLight->SetIntensity(brightness, brightness, brightness);
}

void HScene::InitStructureData()
{
	m_mainCamera->UpdateTransformData();
	for (HInt i = 0; i < primitives.size(); i++)
	{
		primitives[i]->UpdateTransformData();
		m_aabb.Merge(primitives[i]->GetAABBWorld());
	}
	for (HInt i = 0; i < debugMsgLines.size(); i++)
	{
		debugMsgLines[i]->UpdateTransformData();
	}
	UpdateAccelerateStructure();
}

void HScene::InitSamplers()
{
	D3D12_SAMPLER_DESC samplerDesc;
	::SecureZeroMemory(&samplerDesc, sizeof(D3D12_SAMPLER_DESC));
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	samplerDesc.MipLODBias = 0;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;

	auto pD3DDevice = m_dxResources->GetD3DDevice();
	pD3DDevice->CreateSampler(&samplerDesc, m_samplerHeap->GetCPUDescriptorHandleForHeapStart());
}

static HFloat xxxxx = 0;
void HScene::Update(ComPtr<ID3D12GraphicsCommandList> pCommandList)
{
	xxxxx += 0.01f;

	for (HInt i = 0; i < (HInt)debugMsgLines.size(); i++)
	{
		//if (debugMsgLines[i]->GetName() == "debugLine")
		//{
		//	debugMsgLines[i]->SetRotation(0.0f, xxxxx + (HFloat)i * 0.1f, 0.0f);
		//}
	}

	UpdatePrimitive(pCommandList);
	UpdateScripts();
	UpdateTransform();
	UpdateConstantBuffer();
}

void HScene::Render(ComPtr<ID3D12GraphicsCommandList> pCommandList, const map<string, ComPtr<ID3D12PipelineState>>& pPSOs)
{
	ID3D12DescriptorHeap* ppHeaps[] = { m_cbvSrvHeap.Get(), m_samplerHeap.Get() };
	pCommandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	HUInt primitiveCount = (HUInt)primitives.size();
	HUInt debugMsgLineCount = (HUInt)debugMsgLines.size();
	HUInt renderCount = primitiveCount + debugMsgLineCount;
	HUInt cbvIndex = DXResource::c_frameCount * renderCount;

	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle(m_cbvSrvHeap->GetGPUDescriptorHandleForHeapStart());

	for (HUInt i = 0; i < primitiveCount; i++)
	{
		HUInt cbvIndex = m_dxResources->GetCurrentFrameIndex() * renderCount + i;
		pCommandList->SetGraphicsRootDescriptorTable(0, gpuHandle);
		gpuHandle.Offset(m_cbvDescriptorSize);
		pCommandList->SetGraphicsRootDescriptorTable(1, gpuHandle);
		gpuHandle.Offset(m_cbvDescriptorSize);
		primitives[i]->Render(pCommandList);

		auto pShape = dynamic_pointer_cast<HShape>(primitives[i]);
		if (pShape && pShape->GetMaterial()->TextureEnable())
		{
			string shapeTextureName = pShape->GetMaterial()->GetTextureName();
			gpuHandle.Offset(m_cbvDescriptorSize);
			pCommandList->SetGraphicsRootDescriptorTable(3, gpuHandle);
		}
	}

	pCommandList->SetPipelineState(pPSOs.at("wireFrame").Get());
	for (HUInt i = 0; i < debugMsgLineCount; i++)
	{
		HUInt cbvIndex = m_dxResources->GetCurrentFrameIndex() * renderCount + primitiveCount + i;
		pCommandList->SetGraphicsRootDescriptorTable(0, gpuHandle);
		gpuHandle.Offset(m_cbvDescriptorSize);
		pCommandList->SetGraphicsRootDescriptorTable(1, gpuHandle);
		gpuHandle.Offset(m_cbvDescriptorSize);
		debugMsgLines[i]->Render(pCommandList);
	}

	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle(m_cbvSrvHeap->GetGPUDescriptorHandleForHeapStart());
	pCommandList->SetGraphicsRootDescriptorTable(2, gpuHandle);
	gpuHandle.Offset(m_cbvDescriptorSize);
}

void HScene::OnMouseDown(HEventArg eArg)
{
	Ray ray = m_mainCamera->GenerateRay(HFloat(eArg.X), HFloat(eArg.Y));
	unique_ptr<HDefaultSampler> sampler = make_unique<HDefaultSampler>(1, 1, false, 4);
	//printf("orig: %f, %f, %f  dir: %f, %f, %f\n", ray.GetOrigin().x, ray.GetOrigin().y, ray.GetOrigin().z, ray.GetDirection().x, ray.GetDirection().y, ray.GetDirection().z);
	WhittedIntegrator whi;
	vector<Segment> rayTracePath;
	HFloat3 L = whi.Li(ray, *sampler, *this, 0, &rayTracePath);

#ifdef _DEBUG
	//for (HInt i = 0; i < rayTracePath.size(); i++)
	//{
	//	shared_ptr<HLine> pLine = m_sceneManager->CreateSegment("debugLine", rayTracePath[i].point1, rayTracePath[i].point2);
	//	debugMsgLines.push_back(pLine);
	//}
#endif //_DEBUG

	printf("X: %d, Y: %d, R: %f, G: %f, B: %f\n", eArg.X, eArg.Y, L.x, L.y, L.z);
}

void HScene::OnKeyDown(HEventArg eArg)
{  
	//if (HBII->KeyDown('B'))
	//{
	//	auto pShape = m_sceneManager->CreateBox("box big");
	//	pShape->SetTranslation(-3.0f, 2.5f, -4.0f + xxxxx * 1.0f);
	//	pShape->SetScale(5.0f, 5.0f, 5.0f);
	//	pShape->SetRotation(0.0f, -0.3f, 0.0f);
	//	pShape->SetMaterial(m_sceneManager->CreateMatteMaterial(HFloat3(1.0f, 0.8f, 0.6f), 90.0f));
	//	return;
	//}

	if (HBII->KeyDown('G'))
		MakeBMPImage();
}

void HScene::OnKeyUp(HEventArg eArg)
{
	//printf("-1s.\n");
}

bool HScene::Intersect(Ray worldRay, SurfaceInteraction * out_isect, HInt* out_hitShapeIndex, HFloat tMax) const
{
	*out_hitShapeIndex = -1;
	m_bvhTree->Intersect(worldRay, out_isect, out_hitShapeIndex, tMax);
	return *out_hitShapeIndex != -1;
}

bool HScene::IntersectP(Ray worldRay) const
{
	//m_bvhTree->Intersect(worldRay);
	return false;
}

void HScene::MakeBMPImage()
{
	HInt2 screenSize = { (HInt)m_dxResources->GetOutputSize().x, (HInt)m_dxResources->GetOutputSize().y };
	HInt2 tileSingleSize(32, 32);
	HInt2 tileCount(screenSize.x / tileSingleSize.x + 1, screenSize.y / tileSingleSize.y + 1);
	HInt tileSampleCount = tileCount.x * tileCount.y;
	HInt sampleCount = screenSize.x * screenSize.y;

	ImageBMPData* pRGB = new ImageBMPData[sampleCount];
	memset(pRGB, 0, sizeof(ImageBMPData) * sampleCount);

	printf("生成BMP位图...\n");
	auto time_st = GetTickCount64();

	thread* threads = new thread[tileCount.x * tileCount.y];

	m_makingProcessIndex = 0;
	for (HInt i = 0; i < tileCount.x; i++)
	{
		for (HInt j = 0; j < tileCount.y; j++)
		{
			HInt count = i * tileCount.y + j;
			threads[count] = thread(&HScene::MakeBMPImageTile, this, i, j, tileSingleSize, tileSampleCount, pRGB);
		}
	}

	for (HInt i = 0; i < tileCount.x; i++)
	{
		for (HInt j = 0; j < tileCount.y; j++)
		{
			HInt count = i * tileCount.y + j;
			threads[count].join();
		}
	}

	delete[] threads;
	threads = nullptr;

	//生成BMP图片
	ImageGenerator::GenerateImageBMP((BYTE*)pRGB, screenSize.x, screenSize.y, "D:\\rgb.bmp");

	auto time_ed = GetTickCount64();
	printf("done. 用时：%.2f 秒\n", (HFloat)(time_ed - time_st) / 1000.0f);
}

void HScene::MakeBMPImageTile(HInt tileX, HInt tileY, HInt2 tilesize, HInt tileSampleCount, ImageBMPData* pRGB)
{
	unique_ptr<HDefaultSampler> sampler = make_unique<HDefaultSampler>(1, 1, false, 4);

	HInt2 screenSize = { (HInt)m_dxResources->GetOutputSize().x, (HInt)m_dxResources->GetOutputSize().y };
	for (HInt i = 0; i < tilesize.x; i++)
	{
		for (HInt j = 0; j < tilesize.y; j++)
		{
			HInt2 pixel(tileX * tilesize.x + i, tileY * tilesize.y + j);
			
			if (pixel.y >= screenSize.y || pixel.x >= screenSize.x)
				continue;

			unique_ptr<HSampler> tileSampler = sampler->Clone();

			//printf("%d %d\n", x, y);
			tileSampler->GenerateSampleData(pixel);

			HFloat3 L(0.0f);
			do
			{
				Ray ray = m_mainCamera->GenerateRay((HFloat)pixel.x, (HFloat)pixel.y);
				WhittedIntegrator whi;
				L += whi.Li(ray, *tileSampler, *this, 0, nullptr);
				//printf("R: %f, G: %f, B: %f\n", L.x, L.y, L.z);
			} while (tileSampler->NextSample());

			HInt3 resultRGB(L.x > 1.0f ? 255 : (HInt)(L.x * 255.0f),
				L.y > 1.0f ? 255 : (HInt)(L.y * 255.0f),
				L.z > 1.0f ? 255 : (HInt)(L.z * 255.0f));
			
			HInt rgbIdx = (screenSize.y - pixel.y - 1) * screenSize.x + pixel.x;
			pRGB[rgbIdx].r += resultRGB.x;
			pRGB[rgbIdx].g += resultRGB.y;
			pRGB[rgbIdx].b += resultRGB.z;
		}
	}

	m_makingProcessIndex++;
	HFloat percent = ((HFloat)m_makingProcessIndex / (HFloat)tileSampleCount) * 100.0f;
	printf("\r%.2f%%..", percent < 100.0f ? percent : 100.0f);
}

void HScene::UpdateDescriptors()
{
	auto pD3DDevice = m_dxResources->GetD3DDevice();

	HUInt primitiveCount = (HUInt)primitives.size();
	HUInt debugMsgLineCount = (HUInt)debugMsgLines.size();
	HUInt renderCount = primitiveCount + debugMsgLineCount;

	// 为常量缓冲区创建新的描述符堆。
	ComPtr<ID3D12DescriptorHeap>	pNewCbvSrvHeap;
	{
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
		heapDesc.NumDescriptors = DXResource::c_frameCount * renderCount * 2 + 1;
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		// 此标志指示此描述符堆可以绑定到管道，并且其中包含的描述符可以由根表引用。
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		DX::ThrowIfFailed(pD3DDevice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&pNewCbvSrvHeap)));

		DX::NAME_D3D12_OBJECT(pNewCbvSrvHeap);
	}

	CD3DX12_CPU_DESCRIPTOR_HANDLE cbvCpuHandle(pNewCbvSrvHeap->GetCPUDescriptorHandleForHeapStart());
	for (HUInt n = 0; n < DXResource::c_frameCount; n++)
	{
		for (HUInt i = 0; i < primitiveCount; i++)
		{
			HInt heapIndex = n * renderCount + i;
			D3D12_GPU_VIRTUAL_ADDRESS cbvGpuAddress = primitives[i]->GetConstantBuffer()->GetGPUVirtualAddress();

			// 将数据写入该资源
			D3D12_CONSTANT_BUFFER_VIEW_DESC desc;
			desc.SizeInBytes = 256;
			desc.BufferLocation = cbvGpuAddress;
			pD3DDevice->CreateConstantBufferView(&desc, cbvCpuHandle);
			cbvCpuHandle.Offset(m_cbvDescriptorSize);

			desc.BufferLocation += 256;
			pD3DDevice->CreateConstantBufferView(&desc, cbvCpuHandle);
			cbvCpuHandle.Offset(m_cbvDescriptorSize);

			auto pShape = dynamic_pointer_cast<HShape>(primitives[i]);
			if (pShape)
			{
				if (pShape->GetMaterial()->TextureEnable())
				{
					string shapeTextureName = pShape->GetMaterial()->GetTextureName();
					shared_ptr<HTexture> pTexture = m_textureMap[shapeTextureName];

					D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc;
					::SecureZeroMemory(&srvDesc, sizeof(D3D12_SHADER_RESOURCE_VIEW_DESC));
					srvDesc.Format = pTexture->resource.Get()->GetDesc().Format;
					srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
					srvDesc.Texture2D.MipLevels = 1;
					pD3DDevice->CreateShaderResourceView(pTexture->resource.Get(), &srvDesc, cbvCpuHandle);
					cbvCpuHandle.Offset(m_cbvDescriptorSize);
				}
			}
		}

		for (HUInt i = 0; i < debugMsgLineCount; i++)
		{
			D3D12_GPU_VIRTUAL_ADDRESS cbvGpuAddress = debugMsgLines[i]->GetConstantBuffer()->GetGPUVirtualAddress();
			CD3DX12_CPU_DESCRIPTOR_HANDLE cbvCpuHandle(pNewCbvSrvHeap->GetCPUDescriptorHandleForHeapStart());

			// 将数据写入该资源
			D3D12_CONSTANT_BUFFER_VIEW_DESC desc;
			desc.SizeInBytes = 256;
			desc.BufferLocation = cbvGpuAddress;
			pD3DDevice->CreateConstantBufferView(&desc, cbvCpuHandle);
			cbvCpuHandle.Offset(m_cbvDescriptorSize);

			cbvCpuHandle.Offset(m_cbvDescriptorSize);
			desc.BufferLocation += 256;
			pD3DDevice->CreateConstantBufferView(&desc, cbvCpuHandle);
			cbvCpuHandle.Offset(m_cbvDescriptorSize);
		}
	}

	D3D12_GPU_VIRTUAL_ADDRESS cbvGpuAddress = m_mainCamera->GetConstantBuffer()->GetGPUVirtualAddress();
	cbvCpuHandle.Offset(m_cbvDescriptorSize);

	D3D12_CONSTANT_BUFFER_VIEW_DESC desc;
	desc.SizeInBytes = 256;
	desc.BufferLocation = cbvGpuAddress;
	pD3DDevice->CreateConstantBufferView(&desc, cbvCpuHandle);

	//m_cbvHeap->Release();
	m_dxResources->WaitForGpu();
	m_cbvSrvHeap = pNewCbvSrvHeap;
}

void HScene::UpdateAccelerateStructure()
{
	m_bvhTree = new HBVHTree(shared_from_this());
	m_bvhTree->BuildTreesWithScene(HBVHSplitMode::SplitCount);
}

void HScene::UpdatePrimitive(ComPtr<ID3D12GraphicsCommandList> pCommandList)
{
	for (auto it = m_preLoadList.begin(); it != m_preLoadList.end(); it++)
	{
		(*it)->GeneratePrimitiveBuffer(pCommandList);
	}

	m_preLoadList.clear();

	UpdateDescriptors();
}

void HScene::UpdateScripts()
{
	for (auto it = scripts.begin(); it != scripts.end(); it++)
	{
		(*it)->Update();
	}
}

void HScene::UpdateTransform()
{
	m_mainCamera->UpdateTransformData();

	HUInt primitiveCount = (HUInt)primitives.size();
	HUInt debugMsgLineCount = (HUInt)debugMsgLines.size();
	HUInt renderCount = primitiveCount + debugMsgLineCount;

	for (HUInt i = 0; i < primitiveCount; i++)
	{
		primitives[i]->UpdateTransformData();
	}

	for (HUInt i = 0; i < debugMsgLineCount; i++)
	{
		debugMsgLines[i]->UpdateTransformData();
	}
}

void HScene::UpdateConstantBuffer()
{
	m_mainCamera->Update();

	HUInt primitiveCount = (HUInt)primitives.size();
	HUInt debugMsgLineCount = (HUInt)debugMsgLines.size();

	for (HUInt i = 0; i < primitiveCount; i++) primitives[i]->Update();
	for (HUInt i = 0; i < debugMsgLineCount; i++) debugMsgLines[i]->Update();
}
