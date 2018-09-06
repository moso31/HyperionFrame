#include "SceneRenderer.h"

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

	LoadSceneAssets();

	// 关闭命令列表并执行它，以开始将顶点/索引缓冲区复制到 GPU 的默认堆中。
	ThrowIfFailed(m_commandList->Close());
	ID3D12CommandList* ppCommandLists[] = { m_commandList.Get() };
	m_dxResources->GetCommandQueue()->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	// 等待命令列表完成执行；顶点/索引缓冲区需要在上载资源超出范围之前上载到 GPU。
	m_dxResources->WaitForGpu();
}

void SceneRenderer::LoadSceneAssets()
{
	m_test_mainCamera = new Camera(m_dxResources);
	m_test_mainCamera->Init();

	m_test_box = new Box(m_dxResources, m_test_mainCamera);
	m_test_box->Init();

	m_test_box_2 = new Box(m_dxResources, m_test_mainCamera);
	m_test_box_2->Init();

	m_test_box_2->SetTranslation(10.0f, 0.0f, -10.0f);
}

void SceneRenderer::WindowSizeChanged()
{
	m_test_mainCamera->OnResize();
}

void SceneRenderer::Update()
{
	static float x = 0;
	x += 0.01f;
	m_test_box->SetRotation(0.0f, x, 0.0f);
	m_test_box->Update();
	m_test_box_2->SetRotation(0.0f, 0.0f, x);
	m_test_box_2->Update();
}

bool SceneRenderer::Render()
{
	ThrowIfFailed(m_dxResources->GetCommandAllocator()->Reset());

	// 调用 ExecuteCommandList() 后可随时重置命令列表。
	ThrowIfFailed(m_commandList->Reset(m_dxResources->GetCommandAllocator(), m_pipelineState.Get()));

	PIXBeginEvent(m_commandList.Get(), 0, L"Draw the cube");
	{
		// 设置要由此帧使用的图形根签名和描述符堆。
		m_commandList->SetGraphicsRootSignature(m_rootSignature.Get());
		ID3D12DescriptorHeap* ppHeaps[] = { m_cbvHeap.Get() };
		m_commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

		// 将当前帧的常量缓冲区绑定到管道。
		CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle(m_cbvHeap->GetGPUDescriptorHandleForHeapStart(), m_dxResources->GetCurrentFrameIndex(), m_cbvDescriptorSize);
		m_commandList->SetGraphicsRootDescriptorTable(0, gpuHandle);

		// 设置视区和剪刀矩形。
		D3D12_VIEWPORT viewport = m_dxResources->GetScreenViewport();
		m_commandList->RSSetViewports(1, &viewport);
		m_commandList->RSSetScissorRects(1, &(m_camera->GetScissorRect()));

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

		m_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_commandList->IASetVertexBuffers(0, 1, &m_vertexBufferView);
		m_commandList->IASetIndexBuffer(&m_indexBufferView);
		m_commandList->DrawIndexedInstanced(36, 1, 0, 0, 0);

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

	//m_test_box->Render();
	//m_test_box_2->Render();
	return true;
}
