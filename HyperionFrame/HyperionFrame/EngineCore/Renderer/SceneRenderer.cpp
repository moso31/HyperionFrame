#include "SceneRenderer.h"
#include "Camera.h"
#include "DirectXHelper.h"

SceneRenderer::SceneRenderer()
{
}

SceneRenderer::SceneRenderer(const std::shared_ptr<DXResource>& dxResource, const std::shared_ptr<HInput>& input) :
	m_dxResources(dxResource),
	m_input(input)
{
	m_test_scene = new HScene(dxResource);

	CreateSceneResources();
}

SceneRenderer::~SceneRenderer()
{
}

void SceneRenderer::CreateSceneResources()
{
	auto d3dDevice = m_dxResources->GetD3DDevice();

	// �������е��������������۵ĸ�ǩ����
	{
		CD3DX12_DESCRIPTOR_RANGE range[3];
		CD3DX12_ROOT_PARAMETER parameter[3];

		range[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
		range[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1);
		range[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 2);
		parameter[0].InitAsDescriptorTable(1, &range[0], D3D12_SHADER_VISIBILITY_VERTEX);
		parameter[1].InitAsDescriptorTable(1, &range[1], D3D12_SHADER_VISIBILITY_VERTEX);
		parameter[2].InitAsDescriptorTable(1, &range[2], D3D12_SHADER_VISIBILITY_PIXEL);

		D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags =
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | // ֻ�����������׶β���Ҫ���ʳ�����������
			D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS;

		CD3DX12_ROOT_SIGNATURE_DESC descRootSignature;
		descRootSignature.Init(_countof(parameter), parameter, 0, nullptr, rootSignatureFlags);

		ComPtr<ID3DBlob> pSignature;
		ComPtr<ID3DBlob> pError;
		DX::ThrowIfFailed(D3D12SerializeRootSignature(&descRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, pSignature.GetAddressOf(), pError.GetAddressOf()));
		DX::ThrowIfFailed(d3dDevice->CreateRootSignature(0, pSignature->GetBufferPointer(), pSignature->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature)));
		DX::NAME_D3D12_OBJECT(m_rootSignature);
	}

	// 1. ������ɫ����
	ComPtr<ID3DBlob> pVertexShader, pPixelShader;
	DX::ThrowIfFailed(D3DReadFileToBlob(L"SampleVertexShader.cso", &pVertexShader));
	DX::ThrowIfFailed(D3DReadFileToBlob(L"SamplePixelShader.cso", &pPixelShader));

	// 2. ������ɫ��֮�󴴽��ܵ�״̬��
	static const D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, 
	};

	D3D12_GRAPHICS_PIPELINE_STATE_DESC state = {};
	state.InputLayout = { inputLayout, _countof(inputLayout) };
	state.pRootSignature = m_rootSignature.Get();
	state.VS = CD3DX12_SHADER_BYTECODE(pVertexShader.Get());
	state.PS = CD3DX12_SHADER_BYTECODE(pPixelShader.Get());
	state.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	state.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	state.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	state.SampleMask = UINT_MAX;
	state.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	state.NumRenderTargets = 1;
	state.RTVFormats[0] = m_dxResources->GetBackBufferFormat();
	state.DSVFormat = m_dxResources->GetDepthBufferFormat();
	state.SampleDesc.Count = 1;

	DX::ThrowIfFailed(m_dxResources->GetD3DDevice()->CreateGraphicsPipelineState(&state, IID_PPV_ARGS(&m_pipelineState)));

	// 3. �����ܵ�״̬֮�����ɾ����ɫ�����ݡ�
	m_vertexShader.clear();
	m_pixelShader.clear();

	// ���������б�
	DX::ThrowIfFailed(d3dDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_dxResources->GetCommandAllocator(), m_pipelineState.Get(), IID_PPV_ARGS(&m_commandList)));
	DX::NAME_D3D12_OBJECT(m_commandList);

	// ����������Դ
	m_test_scene->Init(m_commandList);
	m_input->Attach(m_test_scene);
	UINT primitiveCount = (UINT)m_test_scene->primitives.size();

	// Ϊ���������������������ѡ�
	{
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
		printf("%d\n", primitiveCount);
		heapDesc.NumDescriptors = DXResource::c_frameCount * primitiveCount * 2 + 1;
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		// �˱�־ָʾ���������ѿ��԰󶨵��ܵ����������а����������������ɸ������á�
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		DX::ThrowIfFailed(d3dDevice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&m_cbvHeap)));

		DX::NAME_D3D12_OBJECT(m_cbvHeap);
	}

	CD3DX12_HEAP_PROPERTIES uploadHeapProperties(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC constantBufferDesc = CD3DX12_RESOURCE_DESC::Buffer(DXResource::c_frameCount * primitiveCount * c_alignedConstantBufferSize + 256);
	DX::ThrowIfFailed(d3dDevice->CreateCommittedResource(
		&uploadHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&constantBufferDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_constantBuffer)));

	DX::NAME_D3D12_OBJECT(m_constantBuffer);

	// ӳ�䳣����������
	CD3DX12_RANGE readRange(0, 0);		// ���ǲ������ CPU �ϵĴ���Դ�н��ж�ȡ��
	DX::ThrowIfFailed(m_constantBuffer->Map(0, &readRange, reinterpret_cast<void**>(&m_mappedConstantBuffer)));
	ZeroMemory(m_mappedConstantBuffer, DXResource::c_frameCount * primitiveCount * c_alignedConstantBufferSize + 256);
	// Ӧ�ùر�֮ǰ�����ǲ���Դ�ȡ��ӳ�䡣����Դ����������ʹ���󱣳�ӳ��״̬�ǿ��еġ�

	// ����������������ͼ�Է������ػ�������
	m_cbvDescriptorSize = d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	for (UINT n = 0; n < DXResource::c_frameCount; n++)
	{
		for (UINT i = 0; i < primitiveCount; i++)
		{
			int heapIndex = n * primitiveCount + i;

			D3D12_GPU_VIRTUAL_ADDRESS cbvGpuAddress = m_constantBuffer->GetGPUVirtualAddress();
			cbvGpuAddress += heapIndex * c_alignedConstantBufferSize;

			CD3DX12_CPU_DESCRIPTOR_HANDLE cbvCpuHandle(m_cbvHeap->GetCPUDescriptorHandleForHeapStart());
			cbvCpuHandle.Offset(heapIndex * 2, m_cbvDescriptorSize);

			D3D12_CONSTANT_BUFFER_VIEW_DESC desc;
			desc.SizeInBytes = 256;
			desc.BufferLocation = cbvGpuAddress;
			d3dDevice->CreateConstantBufferView(&desc, cbvCpuHandle);

			cbvCpuHandle.Offset(m_cbvDescriptorSize);
			desc.BufferLocation += 256;
			d3dDevice->CreateConstantBufferView(&desc, cbvCpuHandle);
		}
	}

	int heapIndex = DXResource::c_frameCount * primitiveCount;
	D3D12_GPU_VIRTUAL_ADDRESS cbvGpuAddress = m_constantBuffer->GetGPUVirtualAddress();
	cbvGpuAddress += heapIndex * c_alignedConstantBufferSize;

	CD3DX12_CPU_DESCRIPTOR_HANDLE cbvCpuHandle(m_cbvHeap->GetCPUDescriptorHandleForHeapStart());
	cbvCpuHandle.Offset(heapIndex * 2, m_cbvDescriptorSize);

	D3D12_CONSTANT_BUFFER_VIEW_DESC desc;
	desc.SizeInBytes = 256;
	desc.BufferLocation = cbvGpuAddress;
	d3dDevice->CreateConstantBufferView(&desc, cbvCpuHandle);

	// �ر������б�ִ�������Կ�ʼ������/�������������Ƶ� GPU ��Ĭ�϶��С�
	DX::ThrowIfFailed(m_commandList->Close());
	ID3D12CommandList* ppCommandLists[] = { m_commandList.Get() };
	m_dxResources->GetCommandQueue()->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	// �ȴ������б����ִ�У�����/������������Ҫ��������Դ������Χ֮ǰ���ص� GPU��
	m_dxResources->WaitForGpu();
}

void SceneRenderer::WindowSizeChanged()
{
	m_test_scene->OnResize();
}

void SceneRenderer::Update()
{
	m_test_scene->Update(m_mappedConstantBuffer, c_alignedConstantBufferSize);
}

bool SceneRenderer::Render()
{
	DX::ThrowIfFailed(m_dxResources->GetCommandAllocator()->Reset());

	// ���� ExecuteCommandList() �����ʱ���������б�
	DX::ThrowIfFailed(m_commandList->Reset(m_dxResources->GetCommandAllocator(), m_pipelineState.Get()));

	PIXBeginEvent(m_commandList.Get(), 0, L"Draw the cube");
	{
		// ���������ͼ������Ρ�
		D3D12_VIEWPORT viewport = m_dxResources->GetScreenViewport();
		m_commandList->RSSetViewports(1, &viewport);
		m_commandList->RSSetScissorRects(1, &(m_test_scene->GetMainCamera()->GetScissorRect()));

		// ָʾ����Դ����������Ŀ�ꡣ
		CD3DX12_RESOURCE_BARRIER renderTargetResourceBarrier =
			CD3DX12_RESOURCE_BARRIER::Transition(m_dxResources->GetRenderTarget(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		m_commandList->ResourceBarrier(1, &renderTargetResourceBarrier);

		// ��¼�������
		D3D12_CPU_DESCRIPTOR_HANDLE renderTargetView = m_dxResources->GetRenderTargetView();
		D3D12_CPU_DESCRIPTOR_HANDLE depthStencilView = m_dxResources->GetDepthStencilView();
		m_commandList->ClearRenderTargetView(renderTargetView, DirectX::Colors::CornflowerBlue, 0, nullptr);
		m_commandList->ClearDepthStencilView(depthStencilView, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

		m_commandList->OMSetRenderTargets(1, &renderTargetView, false, &depthStencilView);

		// ����Ҫ�ɴ�֡ʹ�õ�ͼ�θ�ǩ�����������ѡ�
		m_commandList->SetGraphicsRootSignature(m_rootSignature.Get());
		ID3D12DescriptorHeap* ppHeaps[] = { m_cbvHeap.Get() };
		m_commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

		m_test_scene->Render(m_commandList, m_cbvHeap, m_cbvDescriptorSize);

		// ָʾ����Ŀ�����ڻ�����չʾ�����б����ִ�е�ʱ�䡣
		CD3DX12_RESOURCE_BARRIER presentResourceBarrier =
			CD3DX12_RESOURCE_BARRIER::Transition(m_dxResources->GetRenderTarget(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		m_commandList->ResourceBarrier(1, &presentResourceBarrier);
	}
	PIXEndEvent(m_commandList.Get());

	DX::ThrowIfFailed(m_commandList->Close());

	// ִ�������б�
	ID3D12CommandList* ppCommandLists[] = { m_commandList.Get() };
	m_dxResources->GetCommandQueue()->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	return true;
}

void SceneRenderer::OnLButtonClicked(XMINT2 screenXY)
{
	m_test_scene->OnMouseDown(screenXY.x, screenXY.y);
}

void SceneRenderer::OnKeyDown(WPARAM wParam)
{
	m_input->OnKeyDown(wParam);
}
