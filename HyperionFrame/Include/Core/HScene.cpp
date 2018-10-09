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
	m_mainCamera = new Camera(m_dxResources);
	m_mainCamera->Init();
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

	for (size_t i = 0; i < m_transformNodes.size(); i++)
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
			SurfaceInteraction* isect = new SurfaceInteraction();
			printf("Object %d intersected.\n", i);
			int index;
			m_shapes[i]->Intersect(ray, index, isect);
			if (index != -1) printf("hit: %d\n", index);

			isect->ComputeScatterFunctions();
		}
	}
}

Camera * HScene::CreateCamera()
{
	return nullptr;
}

Box * HScene::CreateBox(ComPtr<ID3D12GraphicsCommandList> pCommandList)
{
	auto box = new Box(m_dxResources, m_mainCamera);
	m_transformNodes.push_back(box);
	m_shapes.push_back(box);
	box->Init(pCommandList);
	return box;
}

HPointLight HScene::CreatePointLight()
{
	return HPointLight();
}
