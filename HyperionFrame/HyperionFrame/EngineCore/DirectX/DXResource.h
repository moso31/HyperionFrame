#pragma once
#include "header.h"

class DXResource
{
public:
	static const UINT c_frameCount = 3;		// 使用三重缓冲。

	DXResource() {}
	DXResource(HWND hwnd, DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D32_FLOAT);
	~DXResource();

	void SetWindow(XMFLOAT2 size);

	void CreateDeviceResources();
	void CreateWindowSizeDependentResources();
	void GetHardWareAdapter(IDXGIAdapter1** out_ppAdapter);

	void WaitForGpu();
	void Present();
	void MoveToNextFrame();

	void UpdateRenderTargetSize();

	// 呈现器目标的大小，以像素为单位。
	void SetOutputSize(XMFLOAT2 size) { m_outputSize = size; }
	XMFLOAT2 GetOutputSize() const { return m_outputSize; }
	
	// D3D 访问器。
	ID3D12Device*				GetD3DDevice() const				{ return m_d3dDevice.Get(); }
	IDXGISwapChain3*			GetSwapChain() const				{ return m_swapChain.Get(); }
	ID3D12Resource*				GetRenderTarget() const				{ return m_renderTargets[m_currentFrame].Get(); }
	ID3D12Resource*				GetDepthStencil() const				{ return m_depthStencil.Get(); }
	ID3D12CommandQueue*			GetCommandQueue() const				{ return m_commandQueue.Get(); }
	ID3D12CommandAllocator*		GetCommandAllocator() const			{ return m_commandAllocators[m_currentFrame].Get(); }
	DXGI_FORMAT					GetBackBufferFormat() const			{ return m_backBufferFormat; }
	DXGI_FORMAT					GetDepthBufferFormat() const		{ return m_depthBufferFormat; }
	D3D12_VIEWPORT				GetScreenViewport() const			{ return m_screenViewport; }
	UINT						GetCurrentFrameIndex() const		{ return m_currentFrame; }

	CD3DX12_CPU_DESCRIPTOR_HANDLE GetRenderTargetView() const
	{
		return CD3DX12_CPU_DESCRIPTOR_HANDLE(m_rtvHeap->GetCPUDescriptorHandleForHeapStart(), m_currentFrame, m_rtvDescriptorSize);
	}
	CD3DX12_CPU_DESCRIPTOR_HANDLE GetDepthStencilView() const
	{
		return CD3DX12_CPU_DESCRIPTOR_HANDLE(m_dsvHeap->GetCPUDescriptorHandleForHeapStart());
	}


private:
	HWND m_hwnd;

	UINT m_currentFrame;

	ComPtr<ID3D12Device> m_d3dDevice;
	ComPtr<IDXGIFactory4> m_dxgiFactory;
	ComPtr<IDXGISwapChain3> m_swapChain;
	ComPtr<ID3D12CommandQueue> m_commandQueue;
	ComPtr<ID3D12CommandAllocator> m_commandAllocators[c_frameCount];
	ComPtr<ID3D12Resource> m_renderTargets[c_frameCount];
	ComPtr<ID3D12Resource> m_depthStencil;
	ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
	ComPtr<ID3D12DescriptorHeap> m_dsvHeap;

	DXGI_FORMAT m_backBufferFormat;
	DXGI_FORMAT m_depthBufferFormat;
	D3D12_VIEWPORT m_screenViewport;
	UINT m_rtvDescriptorSize;
	//bool m_deviceRemoved;

	// CPU/GPU 同步。
	ComPtr<ID3D12Fence> m_fence;
	UINT64 m_fenceValues[c_frameCount];
	HANDLE m_fenceEvent;
	
	XMFLOAT2 m_d3dRenderTargetSize;
	XMFLOAT2 m_outputSize;
};

