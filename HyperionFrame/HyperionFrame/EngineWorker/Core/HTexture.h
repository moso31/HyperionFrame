#pragma once
#include "header.h"

using namespace std;

struct HTexture
{
	HTexture() = default;
	HTexture(string name, wstring filePath, ComPtr<ID3D12Resource> resource = nullptr, HUInt8* mappedResource = nullptr) :
		name(name), filePath(filePath), resource(resource), mappedResource(mappedResource), subresourceData(D3D12_SUBRESOURCE_DATA()) {}

	string name;
	wstring filePath;

	ComPtr<ID3D12Resource> resource;
	unique_ptr<HUInt8[]> mappedResource;
	D3D12_SUBRESOURCE_DATA subresourceData;
};