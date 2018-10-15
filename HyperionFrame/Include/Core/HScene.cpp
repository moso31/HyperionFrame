#include "HScene.h"

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
	m_mainCamera->SetTranslation(4.0f, 2.0f, -2.0f);
	m_mainCamera->SetLookAt(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < 3; i++)
	{
		auto box = CreateBox(pCommandList);

		if (i == 0) box->SetTranslation(0.0f, 0.0f, 0.0f);
		if (i == 1) box->SetTranslation(2.0f, 0.0f, -2.0f);
		if (i == 2) box->SetTranslation(-2.0f, 0.0f, -2.0f);
	}
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
	Ray ray = m_mainCamera->GenerateRay(static_cast<float>(screenXY.x), static_cast<float>(screenXY.y));
	//printf("orig: %f, %f, %f  dir: %f, %f, %f\n", ray.GetOrigin().x, ray.GetOrigin().y, ray.GetOrigin().z, ray.GetDirection().x, ray.GetDirection().y, ray.GetDirection().z);

	for (int i = 0; i < GetShapeCount(); i++)
	{
		if (m_shapes[i]->IntersectP(ray))
		{
			SurfaceInteraction isect;
			//printf("Object %d intersected.\n", i);
			int index;
			m_shapes[i]->Intersect(ray, index, &isect);
			//if (index != -1) printf("hit: %d\n", index);
			XMFLOAT3 wo = isect.wo;

			isect.ComputeScatterFunctions();

			for (UINT j = 0; j < m_lights.size(); j++)
			{
				XMFLOAT3 wi;
				VisibilityTester vis;
				m_lights[i]->Sample_Li(isect, wi, &vis);
				XMCOLOR3 f = isect.bsdf->f(wo, wi);
			}
		}
	}
}

Camera * HScene::CreateCamera()
{
	auto camera = new Camera(m_dxResources);
	m_transformNodes.push_back(camera);
	m_cameras.push_back(camera);
	camera->Init();
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
