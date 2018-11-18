#include "DXResource.h"
#include "DirectXHelper.h"

DXResource::DXResource(HWND hwnd, DXGI_FORMAT backBufferFormat, DXGI_FORMAT depthBufferFormat) :
	m_hwnd(hwnd),
	m_currentFrame(0),
	m_screenViewport(),
	m_rtvDescriptorSize(0),
	m_fenceEvent(0),
	m_backBufferFormat(backBufferFormat),
	m_depthBufferFormat(depthBufferFormat),
	m_fenceValues{}
	//m_d3dRenderTargetSize(),
	//m_outputSize(),
	//m_logicalSize(),
	//m_nativeOrientation(DisplayOrientations::None),
	//m_currentOrientation(DisplayOrientations::None),
	//m_dpi(-1.0f),
	//m_effectiveDpi(-1.0f),
	//m_deviceRemoved(false)
{
	CreateDeviceResources();
}


DXResource::~DXResource()
{
}

void DXResource::SetWindow(XMFLOAT2 size)
{
	m_outputSize = size;
	CreateWindowSizeDependentResources();
}

void DXResource::CreateDeviceResources()
{
#if defined(_DEBUG)
	// �����Ŀ���ڵ������ɽ׶Σ���ͨ�� SDK �����õ��ԡ�
	{
		ComPtr<ID3D12Debug> debugController;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
		{
			debugController->EnableDebugLayer();
		}
	}
#endif

	DX::ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&m_dxgiFactory)));

	ComPtr<IDXGIAdapter1> adapter;
	GetHardWareAdapter(&adapter);

	// ���� Direct3D 12 API �豸����
	DX::ThrowIfFailed(D3D12CreateDevice(
		adapter.Get(),					// Ӳ����������
		D3D_FEATURE_LEVEL_11_0,			// ��Ӧ�ÿ���֧�ֵ���͹��ܼ���
		IID_PPV_ARGS(&m_d3dDevice)		// ���ش����� Direct3D �豸��
	));

	// ����������С�
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	DX::ThrowIfFailed(m_d3dDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue)));
	DX::NAME_D3D12_OBJECT(m_commandQueue);

	// Ϊ������Ŀ����ͼ�����ģ����ͼ�����������ѡ�
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
	rtvHeapDesc.NumDescriptors = c_frameCount;
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	DX::ThrowIfFailed(m_d3dDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap)));
	DX::NAME_D3D12_OBJECT(m_rtvHeap);

	m_rtvDescriptorSize = m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	DX::ThrowIfFailed(m_d3dDevice->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&m_dsvHeap)));
	DX::NAME_D3D12_OBJECT(m_dsvHeap);

	for (UINT n = 0; n < c_frameCount; n++)
	{
		DX::ThrowIfFailed(m_d3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocators[n])));
	}

	// ����ͬ������
	DX::ThrowIfFailed(m_d3dDevice->CreateFence(m_fenceValues[m_currentFrame], D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)));
	m_fenceValues[m_currentFrame]++;

	m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	if (m_fenceEvent == nullptr)
	{
		DX::ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
	}
}

// ÿ�θ��Ĵ��ڴ�Сʱ��Ҫ���´�����Щ��Դ��
void DXResource::CreateWindowSizeDependentResources()
{
	// �ȵ���ǰ������ GPU ������ɡ�
	WaitForGpu();

	// ����ض�����ǰ���ڴ�С�����ݣ������������ٵ�Χ��ֵ��
	for (UINT n = 0; n < c_frameCount; n++)
	{
		m_renderTargets[n] = nullptr;
		m_fenceValues[n] = m_fenceValues[m_currentFrame];
	}

	UpdateRenderTargetSize();

	//// �������Ŀ�Ⱥ͸߶ȱ�����ڴ��ڵ�
	//// ���򱾻��Ŀ�Ⱥ͸߶ȡ�������ڲ��ڱ���
	//// ���������ʹ�ߴ練ת��
	//DXGI_MODE_ROTATION displayRotation = ComputeDisplayRotation();

	//bool swapDimensions = displayRotation == DXGI_MODE_ROTATION_ROTATE90 || displayRotation == DXGI_MODE_ROTATION_ROTATE270;
	//m_d3dRenderTargetSize.Width = swapDimensions ? m_outputSize.Height : m_outputSize.Width;
	//m_d3dRenderTargetSize.Height = swapDimensions ? m_outputSize.Width : m_outputSize.Height;

	m_d3dRenderTargetSize = m_outputSize;

	UINT backBufferWidth = lround(m_d3dRenderTargetSize.x);
	UINT backBufferHeight = lround(m_d3dRenderTargetSize.y);

	if (m_swapChain != nullptr)
	{
		// ����������Ѵ��ڣ���������С��
		HRESULT hr = m_swapChain->ResizeBuffers(c_frameCount, backBufferWidth, backBufferHeight, m_backBufferFormat, 0);

		if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
		{
			// ��������κ�ԭ���Ƴ����豸������Ҫ����һ���µ��豸�ͽ�������
			//m_deviceRemoved = true;

			// �������ִ�д˷����������ٲ����´��� DeviceResources��
			return;
		}
		else
		{
			DX::ThrowIfFailed(hr);
		}
	}
	else
	{
		// ����ʹ�������� Direct3D �豸��ͬ���������½�һ����
		//DXGI_SCALING scaling = DisplayMetrics::SupportHighResolutions ? DXGI_SCALING_NONE : DXGI_SCALING_STRETCH;
		DXGI_SCALING scaling = DXGI_SCALING_STRETCH;
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};

		swapChainDesc.Width = backBufferWidth;						// ƥ�䴰�ڵĴ�С��
		swapChainDesc.Height = backBufferHeight;
		swapChainDesc.Format = m_backBufferFormat;
		swapChainDesc.Stereo = false;
		swapChainDesc.SampleDesc.Count = 1;							// �벻Ҫʹ�ö������
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = c_frameCount;					// ʹ�����ػ������̶ȵؼ�С�ӳ١�
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;	// ���� Windows ͨ��Ӧ�ö�����ʹ�� _FLIP_ SwapEffects��
		swapChainDesc.Flags = 0;
		swapChainDesc.Scaling = scaling;
		swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;

		// ȫ��Ļ
		DXGI_MODE_SCANLINE_ORDER modeScalingOrder = DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;
		DXGI_MODE_SCALING modeScaling = DXGI_MODE_SCALING_STRETCHED;
		DXGI_SWAP_CHAIN_FULLSCREEN_DESC swapChainFullScreenDesc = {};
		swapChainFullScreenDesc.RefreshRate.Numerator = 60;
		swapChainFullScreenDesc.RefreshRate.Denominator = 1;
		swapChainFullScreenDesc.Scaling = modeScaling;
		swapChainFullScreenDesc.ScanlineOrdering = modeScalingOrder;
		swapChainFullScreenDesc.Windowed = false;

		ComPtr<IDXGISwapChain1> swapChain;
		DX::ThrowIfFailed(
			m_dxgiFactory->CreateSwapChainForHwnd(
				m_commandQueue.Get(),
				m_hwnd,
				&swapChainDesc,
				nullptr, // &swapChainFullScreenDesc,
				nullptr,
				&swapChain
			)
		);

		DX::ThrowIfFailed(swapChain.As(&m_swapChain));
	}

	//// Ϊ������������ȷ���򣬲�����
	//// ת������Ⱦ����ת��������
	//// ��ʽָ�� 3D ������Ա���������

	//switch (displayRotation)
	//{
	//case DXGI_MODE_ROTATION_IDENTITY:
	//	m_orientationTransform3D = ScreenRotation::Rotation0;
	//	break;

	//case DXGI_MODE_ROTATION_ROTATE90:
	//	m_orientationTransform3D = ScreenRotation::Rotation270;
	//	break;

	//case DXGI_MODE_ROTATION_ROTATE180:
	//	m_orientationTransform3D = ScreenRotation::Rotation180;
	//	break;

	//case DXGI_MODE_ROTATION_ROTATE270:
	//	m_orientationTransform3D = ScreenRotation::Rotation90;
	//	break;

	//default:
	//	throw ref new FailureException();
	//}

	//DX::ThrowIfFailed(
	//	m_swapChain->SetRotation(displayRotation)
	//);

	// ������������̨�������ĳ���Ŀ����ͼ��
	{
		m_currentFrame = m_swapChain->GetCurrentBackBufferIndex();
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvDescriptor(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());
		for (UINT n = 0; n < c_frameCount; n++)
		{
			DX::ThrowIfFailed(m_swapChain->GetBuffer(n, IID_PPV_ARGS(&m_renderTargets[n])));
			m_d3dDevice->CreateRenderTargetView(m_renderTargets[n].Get(), nullptr, rtvDescriptor);
			rtvDescriptor.Offset(m_rtvDescriptorSize);

			WCHAR name[25];
			if (swprintf_s(name, L"m_renderTargets[%u]", n) > 0)
			{
				DX::SetName(m_renderTargets[n].Get(), name);
			}
		}
	}

	// �������ģ�ߺ���ͼ��
	{
		D3D12_HEAP_PROPERTIES depthHeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

		D3D12_RESOURCE_DESC depthResourceDesc = CD3DX12_RESOURCE_DESC::Tex2D(m_depthBufferFormat, backBufferWidth, backBufferHeight, 1, 1);
		depthResourceDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

		CD3DX12_CLEAR_VALUE depthOptimizedClearValue(m_depthBufferFormat, 1.0f, 0);

		DX::ThrowIfFailed(m_d3dDevice->CreateCommittedResource(
			&depthHeapProperties,
			D3D12_HEAP_FLAG_NONE,
			&depthResourceDesc,
			D3D12_RESOURCE_STATE_DEPTH_WRITE,
			&depthOptimizedClearValue,
			IID_PPV_ARGS(&m_depthStencil)
		));

		DX::NAME_D3D12_OBJECT(m_depthStencil);

		D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
		dsvDesc.Format = m_depthBufferFormat;
		dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Flags = D3D12_DSV_FLAG_NONE;

		m_d3dDevice->CreateDepthStencilView(m_depthStencil.Get(), &dsvDesc, m_dsvHeap->GetCPUDescriptorHandleForHeapStart());
	}

	// ��������ȷ���������ڵ� 3D ��Ⱦ������
	//m_screenViewport = { 0.0f, 0.0f, m_d3dRenderTargetSize.Width, m_d3dRenderTargetSize.Height, 0.0f, 1.0f };
	m_screenViewport = { 0.0f, 0.0f, m_outputSize.x, m_outputSize.y, 0.0f, 1.0f };
}

void DXResource::GetHardWareAdapter(IDXGIAdapter1 ** out_ppAdapter)
{
	ComPtr<IDXGIAdapter1> adapter;
	*out_ppAdapter = nullptr;

	for (UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != m_dxgiFactory->EnumAdapters1(adapterIndex, &adapter); adapterIndex++)
	{
		DXGI_ADAPTER_DESC1 desc;
		adapter->GetDesc1(&desc);

		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			// ��Ҫѡ�������������������������
			continue;
		}

		// ����������Ƿ�֧�� Direct3D 12������Ҫ����
		// ��Ϊʵ���豸��
		if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
		{
			break;
		}
	}

	*out_ppAdapter = adapter.Detach();
}

// �ȴ������ GPU ������ɡ�
void DXResource::WaitForGpu()
{
	// �ڶ����а����ź����
	DX::ThrowIfFailed(m_commandQueue->Signal(m_fence.Get(), m_fenceValues[m_currentFrame]));

	// �ȴ���ԽΧ����
	DX::ThrowIfFailed(m_fence->SetEventOnCompletion(m_fenceValues[m_currentFrame], m_fenceEvent));
	WaitForSingleObjectEx(m_fenceEvent, INFINITE, FALSE);

	// �Ե�ǰ֡����Χ��ֵ��
	m_fenceValues[m_currentFrame]++;
}

// ����������������ʾ����Ļ�ϡ�
void DXResource::Present()
{
	// ��һ������ָʾ DXGI ������ֱֹ�� VSync����ʹӦ�ó���
	// ����һ�� VSync ǰ�������ߡ��⽫ȷ�����ǲ����˷��κ�������Ⱦ
	// �Ӳ�������Ļ����ʾ��֡��
	HRESULT hr = m_swapChain->Present(1, 0);

	//// ���ͨ���Ͽ����ӻ��������������Ƴ����豸�������
	//// �������´��������豸��Դ��
	//if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
	//{
	//	m_deviceRemoved = true;
	//}
	//else
	{
		DX::ThrowIfFailed(hr);

		MoveToNextFrame();
	}
}

// ׼��������һ֡��
void DXResource::MoveToNextFrame()
{
	// �ڶ����а����ź����
	const UINT64 currentFenceValue = m_fenceValues[m_currentFrame];
	DX::ThrowIfFailed(m_commandQueue->Signal(m_fence.Get(), currentFenceValue));

	// ���֡������
	m_currentFrame = m_swapChain->GetCurrentBackBufferIndex();

	// �����һ֡�Ƿ�׼����������
	if (m_fence->GetCompletedValue() < m_fenceValues[m_currentFrame])
	{
		DX::ThrowIfFailed(m_fence->SetEventOnCompletion(m_fenceValues[m_currentFrame], m_fenceEvent));
		WaitForSingleObjectEx(m_fenceEvent, INFINITE, FALSE);
	}

	// Ϊ��һ֡����Χ��ֵ��
	m_fenceValues[m_currentFrame] = currentFenceValue + 1;
}

// ȷ��������Ŀ��ĳߴ缰���Ƿ���С��
void DXResource::UpdateRenderTargetSize()
{
	//m_effectiveDpi = m_dpi;

	//// Ϊ���ӳ��߷ֱ����豸�ϵĵ��ʹ��ʱ�䣬����ֵ���С�ĳ�����Ŀ��
	//// ������ GPU ����ʾ���ʱ���������
	//if (!DisplayMetrics::SupportHighResolutions && m_dpi > DisplayMetrics::DpiThreshold)
	//{
	//	float width = DX::ConvertDipsToPixels(m_logicalSize.Width, m_dpi);
	//	float height = DX::ConvertDipsToPixels(m_logicalSize.Height, m_dpi);

	//	// ���豸Ϊ����ʱ���߶ȴ��ڿ�ȡ���
	//	// �ϴ�ߴ�������ֵ���бȽϣ�����С�ߴ�
	//	// ��߶���ֵ���бȽϡ�
	//	if (max(width, height) > DisplayMetrics::WidthThreshold && min(width, height) > DisplayMetrics::HeightThreshold)
	//	{
	//		// Ϊ������Ӧ�ã����Ǹ�������Ч DPI���߼���С���䡣
	//		m_effectiveDpi /= 2.0f;
	//	}
	//}

	//// �����Ҫ�ĳ���Ŀ���С(������Ϊ��λ)��
	//m_outputSize.Width = DX::ConvertDipsToPixels(m_logicalSize.Width, m_effectiveDpi);
	//m_outputSize.Height = DX::ConvertDipsToPixels(m_logicalSize.Height, m_effectiveDpi);

	// ��ֹ������СΪ��� DirectX ���ݡ�
	m_outputSize.x = max(m_outputSize.x, (float)1);
	m_outputSize.y = max(m_outputSize.y, (float)1);
}