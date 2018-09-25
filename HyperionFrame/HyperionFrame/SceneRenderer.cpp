#include "SceneRenderer.h"
#include "HMath.h"

static const UINT c_boxCount = 3;

SceneRenderer::SceneRenderer()
{
}

SceneRenderer::SceneRenderer(const std::shared_ptr<DXResource>& dxResource) :
	m_dxResources(dxResource)
{
	CreateSceneResources();
}

SceneRenderer::~SceneRenderer()
{
}

void SceneRenderer::CreateSceneResources()
{
	auto d3dDevice = m_dxResources->GetD3DDevice();

	// 创建具有单个常量缓冲区槽的根签名。
	{
		CD3DX12_DESCRIPTOR_RANGE range;
		CD3DX12_ROOT_PARAMETER parameter;

		range.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
		parameter.InitAsDescriptorTable(1, &range, D3D12_SHADER_VISIBILITY_VERTEX);

		D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags =
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | // 只有输入汇编程序阶段才需要访问常量缓冲区。
			D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;

		CD3DX12_ROOT_SIGNATURE_DESC descRootSignature;
		descRootSignature.Init(1, &parameter, 0, nullptr, rootSignatureFlags);

		ComPtr<ID3DBlob> pSignature;
		ComPtr<ID3DBlob> pError;
		ThrowIfFailed(D3D12SerializeRootSignature(&descRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, pSignature.GetAddressOf(), pError.GetAddressOf()));
		ThrowIfFailed(d3dDevice->CreateRootSignature(0, pSignature->GetBufferPointer(), pSignature->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature)));
		NAME_D3D12_OBJECT(m_rootSignature);
	}

	// 1. 加载着色器。
	m_vertexShader = FileReader::ReadData(L"SampleVertexShader.cso");
	m_pixelShader = FileReader::ReadData(L"SamplePixelShader.cso");

	// 2. 加载着色器之后创建管道状态。
	static const D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	D3D12_GRAPHICS_PIPELINE_STATE_DESC state = {};
	state.InputLayout = { inputLayout, _countof(inputLayout) };
	state.pRootSignature = m_rootSignature.Get();
	state.VS = CD3DX12_SHADER_BYTECODE(&m_vertexShader[0], m_vertexShader.size());
	state.PS = CD3DX12_SHADER_BYTECODE(&m_pixelShader[0], m_pixelShader.size());
	state.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	state.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	state.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	state.SampleMask = UINT_MAX;
	state.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	state.NumRenderTargets = 1;
	state.RTVFormats[0] = m_dxResources->GetBackBufferFormat();
	state.DSVFormat = m_dxResources->GetDepthBufferFormat();
	state.SampleDesc.Count = 1;

	ThrowIfFailed(m_dxResources->GetD3DDevice()->CreateGraphicsPipelineState(&state, IID_PPV_ARGS(&m_pipelineState)));

	// 3. 创建管道状态之后可以删除着色器数据。
	m_vertexShader.clear();
	m_pixelShader.clear();

	// 创建命令列表。
	ThrowIfFailed(d3dDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_dxResources->GetCommandAllocator(), m_pipelineState.Get(), IID_PPV_ARGS(&m_commandList)));
	NAME_D3D12_OBJECT(m_commandList);

	// 为常量缓冲区创建描述符堆。
	{
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
		heapDesc.NumDescriptors = DXResource::c_frameCount * (c_boxCount);
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		// 此标志指示此描述符堆可以绑定到管道，并且其中包含的描述符可以由根表引用。
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		ThrowIfFailed(d3dDevice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&m_cbvHeap)));

		NAME_D3D12_OBJECT(m_cbvHeap);
	}

	CD3DX12_HEAP_PROPERTIES uploadHeapProperties(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC constantBufferDesc = CD3DX12_RESOURCE_DESC::Buffer(DXResource::c_frameCount * c_alignedConstantBufferSize * c_boxCount);
	ThrowIfFailed(d3dDevice->CreateCommittedResource(
		&uploadHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&constantBufferDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_constantBuffer)));

	NAME_D3D12_OBJECT(m_constantBuffer);

	// 映射常量缓冲区。
	CD3DX12_RANGE readRange(0, 0);		// 我们不打算从 CPU 上的此资源中进行读取。
	ThrowIfFailed(m_constantBuffer->Map(0, &readRange, reinterpret_cast<void**>(&m_mappedConstantBuffer)));
	ZeroMemory(m_mappedConstantBuffer, DXResource::c_frameCount * c_alignedConstantBufferSize * c_boxCount);
	// 应用关闭之前，我们不会对此取消映射。在资源生命周期内使对象保持映射状态是可行的。

	// 创建常量缓冲区视图以访问上载缓冲区。
	m_cbvDescriptorSize = d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	for (int n = 0; n < DXResource::c_frameCount; n++)
	{
		for (int i = 0; i < c_boxCount; i++)
		{
			int heapIndex = n * c_boxCount + i;

			D3D12_GPU_VIRTUAL_ADDRESS cbvGpuAddress = m_constantBuffer->GetGPUVirtualAddress();
			cbvGpuAddress += (n * c_boxCount + i) * c_alignedConstantBufferSize;

			CD3DX12_CPU_DESCRIPTOR_HANDLE cbvCpuHandle(m_cbvHeap->GetCPUDescriptorHandleForHeapStart());
			cbvCpuHandle.Offset(heapIndex, m_cbvDescriptorSize);

			D3D12_CONSTANT_BUFFER_VIEW_DESC desc;
			desc.BufferLocation = cbvGpuAddress;
			desc.SizeInBytes = c_alignedConstantBufferSize;
			d3dDevice->CreateConstantBufferView(&desc, cbvCpuHandle);
		}
	}

	// 创建场景资源
	LoadSceneAssets();

	// 关闭命令列表并执行它，以开始将顶点/索引缓冲区复制到 GPU 的默认堆中。
	ThrowIfFailed(m_commandList->Close());
	ID3D12CommandList* ppCommandLists[] = { m_commandList.Get() };
	m_dxResources->GetCommandQueue()->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	// 等待命令列表完成执行；顶点/索引缓冲区需要在上载资源超出范围之前上载到 GPU。
	m_dxResources->WaitForGpu();

	for (size_t i = 0; i < m_test_boxes.size(); i++)
	{
		m_test_boxes[i]->ReleaseUploadBuffers();
	}
}

void SceneRenderer::LoadSceneAssets()
{
	m_test_mainCamera = new Camera(m_dxResources);
	m_test_mainCamera->Init();
	//m_test_mainCamera->SetTranslation(0.0f, 0.7f, 1.5f);
	//m_test_mainCamera->SetLookAt(0.0f, -0.1f, 0.0f);
	m_test_mainCamera->SetTranslation(4.0f, 2.0f, -2.0f);
	m_test_mainCamera->SetLookAt(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < c_boxCount; i++)
	{
		auto box = new Box(m_dxResources, m_test_mainCamera);
		m_test_boxes.push_back(box);
		box->Init(m_commandList);

		if (i == 0) box->SetTranslation(0.0f, 0.0f, 0.0f);
		if (i == 1) box->SetTranslation(2.0f, 0.0f, -2.0f);
		if (i == 2) box->SetTranslation(-2.0f, 0.0f, -2.0f);
	}
}

void SceneRenderer::WindowSizeChanged()
{
	m_test_mainCamera->OnResize();
}

void SceneRenderer::Update()
{
	static float x = 0;
	x += 0.01f;

	m_test_mainCamera->Update();

	for (size_t i = 0; i < m_test_boxes.size(); i++)
	{
		m_test_boxes[i]->SetRotation(0.0f, x, 0.0f);
		UINT8* destination = m_mappedConstantBuffer + ((m_dxResources->GetCurrentFrameIndex() * c_boxCount + i) * c_alignedConstantBufferSize);
		m_test_boxes[i]->Update(destination);
	}
}

bool SceneRenderer::Render()
{
	ThrowIfFailed(m_dxResources->GetCommandAllocator()->Reset());

	// 调用 ExecuteCommandList() 后可随时重置命令列表。
	ThrowIfFailed(m_commandList->Reset(m_dxResources->GetCommandAllocator(), m_pipelineState.Get()));

	PIXBeginEvent(m_commandList.Get(), 0, L"Draw the cube");
	{
		// 设置视区和剪刀矩形。
		D3D12_VIEWPORT viewport = m_dxResources->GetScreenViewport();
		m_commandList->RSSetViewports(1, &viewport);
		m_commandList->RSSetScissorRects(1, &(m_test_mainCamera->GetScissorRect()));

		// 指示此资源会用作呈现目标。
		CD3DX12_RESOURCE_BARRIER renderTargetResourceBarrier =
			CD3DX12_RESOURCE_BARRIER::Transition(m_dxResources->GetRenderTarget(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		m_commandList->ResourceBarrier(1, &renderTargetResourceBarrier);

		// 记录绘制命令。
		D3D12_CPU_DESCRIPTOR_HANDLE renderTargetView = m_dxResources->GetRenderTargetView();
		D3D12_CPU_DESCRIPTOR_HANDLE depthStencilView = m_dxResources->GetDepthStencilView();
		m_commandList->ClearRenderTargetView(renderTargetView, DirectX::Colors::CornflowerBlue, 0, nullptr);
		m_commandList->ClearDepthStencilView(depthStencilView, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

		m_commandList->OMSetRenderTargets(1, &renderTargetView, false, &depthStencilView);

		// 设置要由此帧使用的图形根签名和描述符堆。
		m_commandList->SetGraphicsRootSignature(m_rootSignature.Get());
		ID3D12DescriptorHeap* ppHeaps[] = { m_cbvHeap.Get() };
		m_commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

		// 将当前帧的常量缓冲区绑定到管道。
		CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle(m_cbvHeap->GetGPUDescriptorHandleForHeapStart(), m_dxResources->GetCurrentFrameIndex() * c_boxCount, m_cbvDescriptorSize);

		for (size_t i = 0; i < m_test_boxes.size(); i++)
		{
			UINT cbvIndex = m_dxResources->GetCurrentFrameIndex() * c_boxCount + (UINT)i;
			CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle(m_cbvHeap->GetGPUDescriptorHandleForHeapStart());
			gpuHandle.Offset(cbvIndex, m_cbvDescriptorSize);

			m_commandList->SetGraphicsRootDescriptorTable(0, gpuHandle);
			m_test_boxes[i]->Render(m_commandList);
		}

		// 指示呈现目标现在会用于展示命令列表完成执行的时间。
		CD3DX12_RESOURCE_BARRIER presentResourceBarrier =
			CD3DX12_RESOURCE_BARRIER::Transition(m_dxResources->GetRenderTarget(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		m_commandList->ResourceBarrier(1, &presentResourceBarrier);
	}
	PIXEndEvent(m_commandList.Get());

	ThrowIfFailed(m_commandList->Close());

	// 执行命令列表。
	ID3D12CommandList* ppCommandLists[] = { m_commandList.Get() };
	m_dxResources->GetCommandQueue()->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	return true;
}

void SceneRenderer::OnLButtonClicked(XMINT2 screenXY)
{
	Ray ray = m_test_mainCamera->GenerateRay(static_cast<float>(screenXY.x), static_cast<float>(screenXY.y));
	//printf("orig: %f, %f, %f  dir: %f, %f, %f\n", ray.GetOrigin().x, ray.GetOrigin().y, ray.GetOrigin().z, ray.GetDirection().x, ray.GetDirection().y, ray.GetDirection().z);

	for (int i = 0; i < c_boxCount; i++)
	{
		if (m_test_boxes[i]->IntersectP(ray))
		{
			SurfaceInteraction* isect = new SurfaceInteraction();
			//printf("Object %d intersected.\n", i);
			int index;
			XMFLOAT3 hitPos;
			m_test_boxes[i]->Intersect(ray, hitPos, index, isect);
			
			//if (index != -1) printf("hit: %d\n", index);

			
		}
	}
}
