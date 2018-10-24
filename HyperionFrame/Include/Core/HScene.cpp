#include "HScene.h"
#include "ImageGenerator.h"
#include "whitted.h"

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

	for (size_t i = 0; i < shapes.size(); i++)
	{
		shapes[i]->SetRotation(0.0f, x, 0.0f);
		UINT8* destination = pMappedConstantBuffer + ((m_dxResources->GetCurrentFrameIndex() * GetShapeCount() + i) * c_alignedConstantBufferSize);
		shapes[i]->Update(destination);
	}
}

void HScene::Render(ComPtr<ID3D12GraphicsCommandList> pCommandList, ComPtr<ID3D12DescriptorHeap> pCbvHeap, UINT cbvDescriptorSize)
{
	for (size_t i = 0; i < shapes.size(); i++)
	{
		UINT cbvIndex = m_dxResources->GetCurrentFrameIndex() * GetShapeCount() + (UINT)i;
		CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle(pCbvHeap->GetGPUDescriptorHandleForHeapStart());
		gpuHandle.Offset(cbvIndex, cbvDescriptorSize);

		pCommandList->SetGraphicsRootDescriptorTable(0, gpuHandle);
		shapes[i]->Render(pCommandList);
	}
}

void HScene::OnMouseDown(int x, int y)
{
	Ray ray = m_mainCamera->GenerateRay(float(x), float(y));
	//printf("orig: %f, %f, %f  dir: %f, %f, %f\n", ray.GetOrigin().x, ray.GetOrigin().y, ray.GetOrigin().z, ray.GetDirection().x, ray.GetDirection().y, ray.GetDirection().z);
	WhittedIntegrator whi;
	XMCOLOR3 L = whi.Li(ray, *this, 0);
	printf("R: %f, G: %f, B: %f\n", L.x, L.y, L.z);
}

void HScene::OnKeyDown(WPARAM wParam)
{
	if (wParam == 'G')
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
				WhittedIntegrator whi;
				XMCOLOR3 L = whi.Li(ray, *this, 0);
				//printf("R: %f, G: %f, B: %f\n", L.x, L.y, L.z);

				XMINT3 resultRGB(L.x > 1.0f ? 255 : (int)(L.x * 255.0f), 
					L.y > 1.0f ? 255 : (int)(L.y * 255.0f),
					L.z > 1.0f ? 255 : (int)(L.z * 255.0f));

				pRGB[(sz.y - iy - 1) * sz.x + ix].r = resultRGB.x;
				pRGB[(sz.y - iy - 1) * sz.x + ix].g = resultRGB.y;
				pRGB[(sz.y - iy - 1) * sz.x + ix].b = resultRGB.z;
			}
		}

		//生成BMP图片
		ImageGenerator::GenerateImageBMP((BYTE*)pRGB, sz.x, sz.y, "D:\\rgb.bmp");
		printf("done.\n");
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

Box * HScene::CreateBox(ComPtr<ID3D12GraphicsCommandList> pCommandList)
{
	auto box = new Box(m_dxResources, m_mainCamera);
	transformNodes.push_back(box);
	shapes.push_back(box);
	box->Init(pCommandList);
	return box;
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
