#pragma once
#include "header.h"

class BMPGenerator
{
public:
	static void MakeBMP(const std::shared_ptr<DXResource>& pDXResource);
	static void MakeBMPTile(int tileX, int tileY, XMINT2 tileSize, int tileSampleCount, ImageBMPData* pRGB);
};