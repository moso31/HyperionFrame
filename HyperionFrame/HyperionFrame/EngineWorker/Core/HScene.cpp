#include "HScene.h"
#include "whitted.h"
#include "HDefaultSampler.h"

#include "Box.h"
#include "Sphere.h"
#include "HMesh.h"
#include "HPointLight.h"
#include "HMatteMaterial.h"
#include "HGlassMaterial.h"
#include "HMirrorMaterial.h"

HScene::HScene()
{
}

HScene::HScene(const std::shared_ptr<DXResource>& dxResource) :
	m_dxResources(dxResource)
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
	m_mainCamera = CreateCamera();
	m_mainCamera->SetTranslation(9.0f, 6.0f, -4.0f);
	m_mainCamera->SetLookAt(0.0f, 0.0f, 0.0f);
	//m_mainCamera->SetRotation(20.0f * H_DEGTORAD, -70.0f * H_DEGTORAD, 0.0f * H_DEGTORAD);

	XMCOLOR3 red = { 1.0f, 0.0f, 0.0f },
		green = { 0.0f, 1.0f, 0.0f },
		blue = { 0.0f, 0.0f, 1.0f },
		yellow = { 1.0f, 1.0f, 0.0f },
		white = { 1.0f, 1.0f, 1.0f },
		mirror_white = { 0.9f, 0.9f, 0.9f },
		gray = { 0.8f, 0.8f, 0.8f };
	float sig = 90.0f;
	HMaterial* mtrl[7] = {
		CreateMatteMaterial(green, sig),
		CreateMatteMaterial(red, sig),
		CreateMatteMaterial(blue, sig),
		CreateMatteMaterial(yellow, sig),
		CreateGlassMaterial(white, white, 1.55f),
		CreateMatteMaterial(gray, sig),
		CreateMirrorMaterial(mirror_white),
	};

	shared_ptr<Shape> shape;
	m_sceneManager = make_shared<HSceneManager>(m_dxResources, pCommandList);

	//shape = m_sceneManager->CreateBox();
	//shape->SetName("wall x-");
	//shape->SetTranslation(-10.0f, 0.0f, 0.0f);
	//shape->SetMaterial(mtrl[2]);
	//shape->SetScale(1.0f, 20.0f, 20.0f);
	//shapes.push_back(shape);

	//shape = m_sceneManager->CreateBox();
	//shape->SetName("wall x+");
	//shape->SetTranslation(+10.0f, 0.0f, 0.0f);
	//shape->SetMaterial(mtrl[0]);
	//shape->SetScale(1.0f, 20.0f, 20.0f);
	//shapes.push_back(shape);

	//shape = m_sceneManager->CreateBox();
	//shape->SetName("wall z-");
	//shape->SetTranslation(0.0f, 0.0f, -10.0f);
	//shape->SetMaterial(mtrl[5]);
	//shape->SetScale(20.0f, 20.0f, 1.0f);
	//shapes.push_back(shape);

	//shape = m_sceneManager->CreateBox();
	//shape->SetName("wall z+");
	//shape->SetTranslation(0.0f, 0.0f, +10.0f);
	//shape->SetMaterial(mtrl[5]);
	//shape->SetScale(20.0f, 20.0f, 1.0f);
	//shapes.push_back(shape);

	shape = m_sceneManager->CreateMesh("D:\\test.fbx");
	shape->SetMaterial(mtrl[4]);
	shapes.push_back(shape);

	shape = m_sceneManager->CreateBox();
	shape->SetName("box big");
	shape->SetTranslation(-3.0f, 2.5f, -4.0f);
	shape->SetScale(5.0f, 5.0f, 5.0f);
	shape->SetRotation(0.0f, -0.3f, 0.0f);
	shape->SetMaterial(mtrl[4]);
	shapes.push_back(shape);

	shape = m_sceneManager->CreateSphere(1.0f, 64, 64);
	shape->SetName("sphere");
	shape->SetTranslation(1.5f, 2.0f, 0.0f);
	shape->SetScale(2.0f, 2.0f, 2.0f);
	shape->SetMaterial(mtrl[4]);
	shapes.push_back(shape);

	shape = m_sceneManager->CreateBox();
	shape->SetName("box small");
	shape->SetTranslation(5.0f, 1.0f, -2.0f);
	shape->SetScale(2.0f, 2.0f, 2.0f);
	shape->SetMaterial(mtrl[4]);
	shapes.push_back(shape);

	int chessSize = 9;
	for (int i = -chessSize; i <= chessSize; i++)
	{
		for (int j = -chessSize; j <= chessSize; j++)
		{
			shape = m_sceneManager->CreateBox();
			if ((i + j) % 2)
			{
				shape->SetTranslation(-i, -0.7f, j);
				shape->SetMaterial(mtrl[1]);
			}
			else
			{
				shape->SetTranslation(-i, -0.5f, j);
				shape->SetMaterial(mtrl[3]);
			}
			shapes.push_back(shape);
		}
	}

	auto pointLight = CreatePointLight();
	XMFLOAT3 lightPos = { 0.0f, 10.0f, 5.0f };
	pointLight->SetTranslation(lightPos.x, lightPos.y, lightPos.z);
	float brightness = 100.0f;
	pointLight->SetIntensity(brightness, brightness, brightness);

	pointLight = CreatePointLight();
	pointLight->SetTranslation(-lightPos.x, lightPos.y, -lightPos.z);
	brightness = 100.0f;
	pointLight->SetIntensity(brightness, brightness, brightness);

	pointLight = CreatePointLight();
	pointLight->SetTranslation(lightPos.z, lightPos.y, lightPos.x);
	brightness = 100.0f;
	pointLight->SetIntensity(brightness, brightness, brightness);

	pointLight = CreatePointLight();
	pointLight->SetTranslation(-lightPos.z, lightPos.y, -lightPos.x);
	brightness = 100.0f;
	pointLight->SetIntensity(brightness, brightness, brightness);

	InitSceneData();
}

void HScene::InitSceneData()
{
	m_mainCamera->UpdateTransformData();
	for (int i = 0; i < shapes.size(); i++)
	{
		shapes[i]->UpdateTransformData();
		m_aabb.Merge(shapes[i]->GetAABBWorld());
	}
	UpdateAccelerateStructure();
}

void HScene::Update(UINT8* pMappedConstantBuffer, const UINT alignedConstantBufferSize)
{
	static float x = 0;
	x += 0.01f;

	m_mainCamera->UpdateTransformData();
	m_mainCamera->Update();
	m_cbEyePos.eyePos = m_mainCamera->GetTranslation();

	UINT8* destination = pMappedConstantBuffer + (DXResource::c_frameCount * GetShapeCount() * alignedConstantBufferSize);
	memcpy(destination, &m_cbEyePos, sizeof(m_cbEyePos));

	for (size_t i = 0; i < shapes.size(); i++)
	{
		UINT8* destination = pMappedConstantBuffer + ((m_dxResources->GetCurrentFrameIndex() * GetShapeCount() + i) * alignedConstantBufferSize);

		shapes[i]->UpdateTransformData();
		shapes[i]->Update(destination);
	}
}

void HScene::Render(ComPtr<ID3D12GraphicsCommandList> pCommandList, ComPtr<ID3D12DescriptorHeap> pCbvHeap, UINT cbvDescriptorSize)
{
	UINT cbvIndex = DXResource::c_frameCount * GetShapeCount();
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle(pCbvHeap->GetGPUDescriptorHandleForHeapStart());
	gpuHandle.Offset(cbvIndex * 2, cbvDescriptorSize);
	pCommandList->SetGraphicsRootDescriptorTable(2, gpuHandle);

	for (size_t i = 0; i < shapes.size(); i++)
	{
		UINT cbvIndex = m_dxResources->GetCurrentFrameIndex() * GetShapeCount() + (UINT)i;
		CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle(pCbvHeap->GetGPUDescriptorHandleForHeapStart());
		gpuHandle.Offset(cbvIndex * 2, cbvDescriptorSize);
		pCommandList->SetGraphicsRootDescriptorTable(0, gpuHandle);
		gpuHandle.Offset(cbvDescriptorSize);
		pCommandList->SetGraphicsRootDescriptorTable(1, gpuHandle);
		shapes[i]->Render(pCommandList);
	}
}

void HScene::OnMouseDown(int x, int y)
{
	Ray ray = m_mainCamera->GenerateRay(float(x), float(y));
	unique_ptr<HDefaultSampler> sampler = make_unique<HDefaultSampler>(1, 1, false, 4);
	//printf("orig: %f, %f, %f  dir: %f, %f, %f\n", ray.GetOrigin().x, ray.GetOrigin().y, ray.GetOrigin().z, ray.GetDirection().x, ray.GetDirection().y, ray.GetDirection().z);
	WhittedIntegrator whi;
	XMCOLOR3 L = whi.Li(ray, *sampler, *this, 0);
	printf("X: %d, Y: %d, R: %f, G: %f, B: %f\n", x, y, L.x, L.y, L.z);
}

void HScene::OnKeyDown(WPARAM wParam)
{
	if (wParam == 'G')
	{
		XMINT2 screenSize = { (int)m_dxResources->GetOutputSize().x, (int)m_dxResources->GetOutputSize().y };
		XMINT2 tileSingleSize(32, 32);
		XMINT2 tileCount(screenSize.x / tileSingleSize.x + 1, screenSize.y / tileSingleSize.y + 1);
		int tileSampleCount = tileCount.x * tileCount.y;
		int sampleCount = screenSize.x * screenSize.y;

		ImageBMPData* pRGB = new ImageBMPData[sampleCount];
		memset(pRGB, 0, sizeof(ImageBMPData) * sampleCount);

		printf("生成BMP位图...\n");
		auto time_st = GetTickCount();

		thread* threads = new thread[tileCount.x * tileCount.y];

		m_makingProcessIndex = 0;
		for (int i = 0; i < tileCount.x; i++)
		{
			for (int j = 0; j < tileCount.y; j++)
			{
				int count = i * tileCount.y + j;
				threads[count] = thread(&HScene::MakeImageTile, this, i, j, tileSingleSize, tileSampleCount, pRGB);
			}
		}

		for (int i = 0; i < tileCount.x; i++)
		{
			for (int j = 0; j < tileCount.y; j++)
			{
				int count = i * tileCount.y + j;
				threads[count].join();
			}
		}

		delete[] threads;
		threads = nullptr;

		//生成BMP图片
		ImageGenerator::GenerateImageBMP((BYTE*)pRGB, screenSize.x, screenSize.y, "D:\\rgb.bmp");

		auto time_ed = GetTickCount();
		printf("done. 用时：%.2f 秒\n", (float)(time_ed - time_st) / 1000.0f);
	}
}

Camera * HScene::CreateCamera()
{
	auto camera = new Camera(m_dxResources);
	transformNodes.push_back(camera);
	cameras.push_back(camera);
	camera->Init(70.0f, 0.01f, 1000.0f);
	return camera;
}

HPointLight * HScene::CreatePointLight()
{
	auto pointLight = new HPointLight();
	transformNodes.push_back(pointLight);
	lights.push_back(pointLight);
	return pointLight;
}

HMatteMaterial * HScene::CreateMatteMaterial(const XMCOLOR3& kd, const float sigma)
{
	auto mat = new HMatteMaterial(kd, sigma);
	materials.push_back(mat);
	return mat;
}

HMirrorMaterial * HScene::CreateMirrorMaterial(const XMCOLOR3 & kr)
{
	auto mat = new HMirrorMaterial(kr);
	materials.push_back(mat);
	return mat;
}

HGlassMaterial * HScene::CreateGlassMaterial(const XMCOLOR3 & Kr, const XMCOLOR3 & Kt, const float eta)
{
	auto mat = new HGlassMaterial(Kr, Kt, eta);
	materials.push_back(mat);
	return mat;
}

bool HScene::Intersect(Ray worldRay, SurfaceInteraction * out_isect, int* out_hitShapeIndex) const
{
	*out_hitShapeIndex = -1;
	m_bvhTree->Intersect(worldRay, out_isect, out_hitShapeIndex);
	return *out_hitShapeIndex != -1;
}

bool HScene::IntersectP(Ray worldRay) const
{
	//m_bvhTree->Intersect(worldRay);
	return false;
}

void HScene::MakeImageTile(int tileX, int tileY, XMINT2 tilesize, int tileSampleCount, ImageBMPData* pRGB)
{
	unique_ptr<HDefaultSampler> sampler = make_unique<HDefaultSampler>(1, 1, false, 4);

	XMINT2 screenSize = { (int)m_dxResources->GetOutputSize().x, (int)m_dxResources->GetOutputSize().y };
	for (int i = 0; i < tilesize.x; i++)
	{
		for (int j = 0; j < tilesize.y; j++)
		{
			XMINT2 pixel(tileX * tilesize.x + i, tileY * tilesize.y + j);
			
			if (pixel.y >= screenSize.y || pixel.x >= screenSize.x)
				continue;

			unique_ptr<HSampler> tileSampler = sampler->Clone();

			//printf("%d %d\n", x, y);
			tileSampler->GenerateSampleData(pixel);

			XMCOLORV LV = XMVectorZero();
			do
			{
				Ray ray = m_mainCamera->GenerateRay((float)pixel.x, (float)pixel.y);
				WhittedIntegrator whi;
				XMCOLOR3 L = whi.Li(ray, *tileSampler, *this, 0);
				//printf("R: %f, G: %f, B: %f\n", L.x, L.y, L.z);
				LV += XMLoadFloat3(&L);
			} while (tileSampler->NextSample());

			XMFLOAT3 result;
			XMStoreFloat3(&result, LV);
			XMINT3 resultRGB(result.x > 1.0f ? 255 : (int)(result.x * 255.0f),
				result.y > 1.0f ? 255 : (int)(result.y * 255.0f),
				result.z > 1.0f ? 255 : (int)(result.z * 255.0f));
			
			int rgbIdx = (screenSize.y - pixel.y - 1) * screenSize.x + pixel.x;
			pRGB[rgbIdx].r += resultRGB.x;
			pRGB[rgbIdx].g += resultRGB.y;
			pRGB[rgbIdx].b += resultRGB.z;
		}
	}

	m_makingProcessIndex++;
	float percent = ((float)m_makingProcessIndex / (float)tileSampleCount) * 100.0f;
	printf("\r%.2f%%..", percent < 100.0f ? percent : 100.0f);
}

void HScene::UpdateAccelerateStructure()
{
	m_bvhTree = new HBVHTree(this);
	m_bvhTree->BuildTreesWithScene(HBVHSplitMode::HLBVH);
}
