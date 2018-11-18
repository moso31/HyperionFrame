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
	// 如果项目处于调试生成阶段，请通过 SDK 层启用调试。
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

	// 创建 Direct3D 12 API 设备对象
	DX::ThrowIfFailed(D3D12CreateDevice(
		adapter.Get(),					// 硬件适配器。
		D3D_FEATURE_LEVEL_11_0,			// 此应用可以支持的最低功能级别。
		IID_PPV_ARGS(&m_d3dDevice)		// 返回创建的 Direct3D 设备。
	));

	// 创建命令队列。
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	DX::ThrowIfFailed(m_d3dDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue)));
	DX::NAME_D3D12_OBJECT(m_commandQueue);

	// 为呈现器目标视图和深度模具视图创建描述符堆。
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

	// 创建同步对象。
	DX::ThrowIfFailed(m_d3dDevice->CreateFence(m_fenceValues[m_currentFrame], D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)));
	m_fenceValues[m_currentFrame]++;

	m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	if (m_fenceEvent == nullptr)
	{
		DX::ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
	}
}

// 每次更改窗口大小时需要重新创建这些资源。
void DXResource::CreateWindowSizeDependentResources()
{
	// 等到以前的所有 GPU 工作完成。
	WaitForGpu();

	// 清除特定于先前窗口大小的内容，并更新所跟踪的围栏值。
	for (UINT n = 0; n < c_frameCount; n++)
	{
		m_renderTargets[n] = nullptr;
		m_fenceValues[n] = m_fenceValues[m_currentFrame];
	}

	UpdateRenderTargetSize();

	//// 交换链的宽度和高度必须基于窗口的
	//// 面向本机的宽度和高度。如果窗口不在本机
	//// 方向，则必须使尺寸反转。
	//DXGI_MODE_ROTATION displayRotation = ComputeDisplayRotation();

	//bool swapDimensions = displayRotation == DXGI_MODE_ROTATION_ROTATE90 || displayRotation == DXGI_MODE_ROTATION_ROTATE270;
	//m_d3dRenderTargetSize.Width = swapDimensions ? m_outputSize.Height : m_outputSize.Width;
	//m_d3dRenderTargetSize.Height = swapDimensions ? m_outputSize.Width : m_outputSize.Height;

	m_d3dRenderTargetSize = m_outputSize;

	UINT backBufferWidth = lround(m_d3dRenderTargetSize.x);
	UINT backBufferHeight = lround(m_d3dRenderTargetSize.y);

	if (m_swapChain != nullptr)
	{
		// 如果交换链已存在，请调整其大小。
		HRESULT hr = m_swapChain->ResizeBuffers(c_frameCount, backBufferWidth, backBufferHeight, m_backBufferFormat, 0);

		if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
		{
			// 如果出于任何原因移除了设备，将需要创建一个新的设备和交换链。
			//m_deviceRemoved = true;

			// 请勿继续执行此方法。会销毁并重新创建 DeviceResources。
			return;
		}
		else
		{
			DX::ThrowIfFailed(hr);
		}
	}
	else
	{
		// 否则，使用与现有 Direct3D 设备相同的适配器新建一个。
		//DXGI_SCALING scaling = DisplayMetrics::SupportHighResolutions ? DXGI_SCALING_NONE : DXGI_SCALING_STRETCH;
		DXGI_SCALING scaling = DXGI_SCALING_STRETCH;
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};

		swapChainDesc.Width = backBufferWidth;						// 匹配窗口的大小。
		swapChainDesc.Height = backBufferHeight;
		swapChainDesc.Format = m_backBufferFormat;
		swapChainDesc.Stereo = false;
		swapChainDesc.SampleDesc.Count = 1;							// 请不要使用多采样。
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = c_frameCount;					// 使用三重缓冲最大程度地减小延迟。
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;	// 所有 Windows 通用应用都必须使用 _FLIP_ SwapEffects。
		swapChainDesc.Flags = 0;
		swapChainDesc.Scaling = scaling;
		swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;

		// 全屏幕
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

	//// 为交换链设置正确方向，并生成
	//// 转换以渲染到旋转交换链。
	//// 显式指定 3D 矩阵可以避免舍入误差。

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

	// 创建交换链后台缓冲区的呈现目标视图。
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

	// 创建深度模具和视图。
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

	// 设置用于确定整个窗口的 3D 渲染视区。
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
			// 不要选择基本呈现驱动程序适配器。
			continue;
		}

		// 检查适配器是否支持 Direct3D 12，但不要创建
		// 仍为实际设备。
		if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
		{
			break;
		}
	}

	*out_ppAdapter = adapter.Detach();
}

// 等待挂起的 GPU 工作完成。
void DXResource::WaitForGpu()
{
	// 在队列中安排信号命令。
	DX::ThrowIfFailed(m_commandQueue->Signal(m_fence.Get(), m_fenceValues[m_currentFrame]));

	// 等待跨越围栏。
	DX::ThrowIfFailed(m_fence->SetEventOnCompletion(m_fenceValues[m_currentFrame], m_fenceEvent));
	WaitForSingleObjectEx(m_fenceEvent, INFINITE, FALSE);

	// 对当前帧递增围栏值。
	m_fenceValues[m_currentFrame]++;
}

// 将交换链的内容显示到屏幕上。
void DXResource::Present()
{
	// 第一个参数指示 DXGI 进行阻止直到 VSync，这使应用程序
	// 在下一个 VSync 前进入休眠。这将确保我们不会浪费任何周期渲染
	// 从不会在屏幕上显示的帧。
	HRESULT hr = m_swapChain->Present(1, 0);

	//// 如果通过断开连接或升级驱动程序移除了设备，则必须
	//// 必须重新创建所有设备资源。
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

// 准备呈现下一帧。
void DXResource::MoveToNextFrame()
{
	// 在队列中安排信号命令。
	const UINT64 currentFenceValue = m_fenceValues[m_currentFrame];
	DX::ThrowIfFailed(m_commandQueue->Signal(m_fence.Get(), currentFenceValue));

	// 提高帧索引。
	m_currentFrame = m_swapChain->GetCurrentBackBufferIndex();

	// 检查下一帧是否准备好启动。
	if (m_fence->GetCompletedValue() < m_fenceValues[m_currentFrame])
	{
		DX::ThrowIfFailed(m_fence->SetEventOnCompletion(m_fenceValues[m_currentFrame], m_fenceEvent));
		WaitForSingleObjectEx(m_fenceEvent, INFINITE, FALSE);
	}

	// 为下一帧设置围栏值。
	m_fenceValues[m_currentFrame] = currentFenceValue + 1;
}

// 确定呈现器目标的尺寸及其是否将缩小。
void DXResource::UpdateRenderTargetSize()
{
	//m_effectiveDpi = m_dpi;

	//// 为了延长高分辨率设备上的电池使用时间，请呈现到较小的呈现器目标
	//// 并允许 GPU 在显示输出时缩放输出。
	//if (!DisplayMetrics::SupportHighResolutions && m_dpi > DisplayMetrics::DpiThreshold)
	//{
	//	float width = DX::ConvertDipsToPixels(m_logicalSize.Width, m_dpi);
	//	float height = DX::ConvertDipsToPixels(m_logicalSize.Height, m_dpi);

	//	// 当设备为纵向时，高度大于宽度。将
	//	// 较大尺寸与宽度阈值进行比较，将较小尺寸
	//	// 与高度阈值进行比较。
	//	if (max(width, height) > DisplayMetrics::WidthThreshold && min(width, height) > DisplayMetrics::HeightThreshold)
	//	{
	//		// 为了缩放应用，我们更改了有效 DPI。逻辑大小不变。
	//		m_effectiveDpi /= 2.0f;
	//	}
	//}

	//// 计算必要的呈现目标大小(以像素为单位)。
	//m_outputSize.Width = DX::ConvertDipsToPixels(m_logicalSize.Width, m_effectiveDpi);
	//m_outputSize.Height = DX::ConvertDipsToPixels(m_logicalSize.Height, m_effectiveDpi);

	// 防止创建大小为零的 DirectX 内容。
	m_outputSize.x = max(m_outputSize.x, (float)1);
	m_outputSize.y = max(m_outputSize.y, (float)1);
}