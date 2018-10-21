#include "HScene.h"
#include "ImageGenerator.h"

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
	m_mainCamera->SetTranslation(0.0f, 2.0f, -4.0f);
	m_mainCamera->SetLookAt(0.0f, 0.0f, 0.0f);

	XMCOLOR3 red = { 1.0f, 0.0f, 0.0f }, green = { 0.0f, 1.0f, 0.0f }, blue = { 0.0f, 0.0f, 1.0f };
	float sig = 90.0f;
	HMatteMaterial* mtrl[3] = {
		CreateMatteMaterial(red, sig),
		CreateMatteMaterial(green, sig),
		CreateMatteMaterial(blue, sig)
	};

	for (int i = 0; i < 3; i++)
	{
		auto box = CreateBox(pCommandList);
		box->SetMaterial(mtrl[i]);

		if (i == 0) box->SetTranslation(0.0f, 0.0f, 0.0f);
		if (i == 1) box->SetTranslation(2.0f, 0.0f, -2.0f);
		if (i == 2) box->SetTranslation(-2.0f, 0.0f, -2.0f);
	}

	auto pointLight = CreatePointLight();
	pointLight->SetTranslation(0.0f, 2.0f, -4.0f);
	pointLight->SetIntensity(25.0f, 25.0f, 25.0f);
}

void HScene::Update(UINT8* pMappedConstantBuffer)
{
	static float x = 0;
	x += 0.01f;

	m_mainCamera->Update();

	for (size_t i = 0; i < m_shapes.size(); i++)
	{
		m_shapes[i]->SetRotation(0.0f, x, 0.0f);
		UINT8* destination = pMappedConstantBuffer + ((m_dxResources->GetCurrentFrameIndex() * GetShapeCount() + i) * c_alignedConstantBufferSize);
		m_shapes[i]->Update(destination);
	}
}

void HScene::Render(ComPtr<ID3D12GraphicsCommandList> pCommandList, ComPtr<ID3D12DescriptorHeap> pCbvHeap, UINT cbvDescriptorSize)
{
	for (size_t i = 0; i < m_shapes.size(); i++)
	{
		UINT cbvIndex = m_dxResources->GetCurrentFrameIndex() * GetShapeCount() + (UINT)i;
		CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle(pCbvHeap->GetGPUDescriptorHandleForHeapStart());
		gpuHandle.Offset(cbvIndex, cbvDescriptorSize);

		pCommandList->SetGraphicsRootDescriptorTable(0, gpuHandle);
		m_shapes[i]->Render(pCommandList);
	}
}

void HScene::OnLButtonClicked(XMINT2 screenXY)
{
	struct TESTBMP
	{
		BYTE b;
		BYTE g;
		BYTE r;
	};  // 定义位图数据
	
	auto szf = m_dxResources->GetOutputSize();
	XMINT2 sz = { (int)szf.x, (int)szf.y };
	int szall = sz.x * sz.y;

	TESTBMP* pRGB = new TESTBMP[sz.y * sz.x];
	for (int i = 0; i < szall; i++)
	{
		pRGB[i].r = pRGB[i].g = pRGB[i].b = 0;
	}

	printf("生成BMP位图中...");

	for (int iy = 0; iy < sz.y; iy++)
	{
		if (iy % 100 == 0)
			printf("-");

		for (int ix = 0; ix < sz.x; ix++)
		{
			//Ray ray = m_mainCamera->GenerateRay(static_cast<float>(screenXY.x), static_cast<float>(screenXY.y));
			//printf("orig: %f, %f, %f  dir: %f, %f, %f\n", ray.GetOrigin().x, ray.GetOrigin().y, ray.GetOrigin().z, ray.GetDirection().x, ray.GetDirection().y, ray.GetDirection().z);
			Ray ray = m_mainCamera->GenerateRay(static_cast<float>(ix), static_cast<float>(iy));
			XMINT3 resultRGB = { 0, 0, 0 };

			bool isHit = false;
			for (int i = 0; i < GetShapeCount(); i++)
			{
				if (m_shapes[i]->IntersectP(ray))
				{
					SurfaceInteraction isect;
					//printf("Object %d intersected.\n", i);
					int index;
					m_shapes[i]->Intersect(ray, index, &isect);
					//printf("hit: %f, %f, %f\n", isect.p.x, isect.p.y, isect.p.z);
					if (index != -1)
						isHit = true;

					XMFLOAT3 wo = isect.wo;

					isect.ComputeScatterFunctions();

					XMCOLORV LV = XMVectorZero();
					for (UINT j = 0; j < m_lights.size(); j++)
					{
						XMFLOAT3 wi;
						VisibilityTester vis;
						XMCOLOR3 Li = m_lights[j]->Sample_Li(isect, wi, &vis);
						XMCOLOR3 f = isect.bsdf->f(wo, wi);

						//if (/*!f.IsBlack() && */vis.Unoccluded(scene))
						{
							XMCOLORV fV = XMLoadFloat3(&f);
							XMCOLORV LiV = XMLoadFloat3(&Li);
							XMVECTOR wiV = XMLoadFloat3(&wi);
							XMVECTOR nV = XMLoadFloat3(&isect.n);

							LV += fV * LiV * XMVectorAbs(XMVector3Dot(wiV, nV));
						}
					}

					XMCOLOR3 L;
					XMStoreFloat3(&L, LV);
					//printf("R: %f, G: %f, B: %f\n", L.x, L.y, L.z);

					resultRGB.x = (int)(L.x * 255.f);
					resultRGB.y = (int)(L.y * 255.f);
					resultRGB.z = (int)(L.z * 255.f);
					if (resultRGB.x > 0xff)
						resultRGB.x = 0xff;
					if (resultRGB.y > 0xff)
						resultRGB.y = 0xff;
					if (resultRGB.z > 0xff)
						resultRGB.z = 0xff;

					break;
				}
			}

			if (isHit)
			{
				pRGB[(sz.y - iy - 1) * sz.x + ix].r = resultRGB.x;
				pRGB[(sz.y - iy - 1) * sz.x + ix].g = resultRGB.y;
				pRGB[(sz.y - iy - 1) * sz.x + ix].b = resultRGB.z;
			}
			else
			{
				pRGB[(sz.y - iy - 1) * sz.x + ix].r = 0;
				pRGB[(sz.y - iy - 1) * sz.x + ix].g = 0;
				pRGB[(sz.y - iy - 1) * sz.x + ix].b = 0;
			}

			//printf("iy: %d, ix: %d, r: %d, g: %d, b: %d\n", iy, ix, resultRGB.x, resultRGB.y, resultRGB.z);
		}
	}

	//生成BMP图片
	ImageGenerator::GenerateImageBMP((BYTE*)pRGB, sz.x, sz.y, "E:\\rgb.bmp");
	printf("done.\n");
}

Camera * HScene::CreateCamera()
{
	auto camera = new Camera(m_dxResources);
	m_transformNodes.push_back(camera);
	m_cameras.push_back(camera);
	camera->Init(70.0f, 0.01f, 1000.0f);
	return camera;
}

Box * HScene::CreateBox(ComPtr<ID3D12GraphicsCommandList> pCommandList)
{
	auto box = new Box(m_dxResources, m_mainCamera);
	m_transformNodes.push_back(box);
	m_shapes.push_back(box);
	box->Init(pCommandList);
	return box;
}

HPointLight * HScene::CreatePointLight()
{
	auto pointLight = new HPointLight();
	m_transformNodes.push_back(pointLight);
	m_lights.push_back(pointLight);
	return pointLight;
}

HMatteMaterial * HScene::CreateMatteMaterial(const XMCOLOR3& kd, const float sigma)
{
	auto mat = new HMatteMaterial(kd, sigma);
	m_materials.push_back(mat);
	return mat;
}
