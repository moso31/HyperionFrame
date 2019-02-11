#include "BMPGenerator.h"
#include "DXResource.h"
#include "ImageGenerator.h"

void BMPGenerator::MakeBMP(const std::shared_ptr<DXResource>& pDXResource)
{
	XMINT2 screenSize = { (int)pDXResource->GetOutputSize().x, (int)pDXResource->GetOutputSize().y };
	XMINT2 tileSingleSize(32, 32);
	XMINT2 tileCount(screenSize.x / tileSingleSize.x + 1, screenSize.y / tileSingleSize.y + 1);
	int tileSampleCount = tileCount.x * tileCount.y;
	int sampleCount = screenSize.x * screenSize.y;

	ImageBMPData* pRGB = new ImageBMPData[sampleCount];
	memset(pRGB, 0, sizeof(ImageBMPData) * sampleCount);

	printf("生成BMP位图...\n");
	auto time_st = GetTickCount();

	thread* threads = new thread[tileCount.x * tileCount.y];

	m_makingProcessIndex = 0;
	for (int i = 0; i < tileCount.x; i++)
	{
		for (int j = 0; j < tileCount.y; j++)
		{
			int count = i * tileCount.y + j;
			threads[count] = thread(&BMPGenerator::MakeBMPTile, i, j, tileSingleSize, tileSampleCount, pRGB);
		}
	}

	for (int i = 0; i < tileCount.x; i++)
	{
		for (int j = 0; j < tileCount.y; j++)
		{
			int count = i * tileCount.y + j;
			threads[count].join();
		}
	}

	delete[] threads;
	threads = nullptr;

	//生成BMP图片
	ImageGenerator::GenerateImageBMP((BYTE*)pRGB, screenSize.x, screenSize.y, "D:\\rgb.bmp");

	auto time_ed = GetTickCount();
	printf("done. 用时：%.2f 秒\n", (float)(time_ed - time_st) / 1000.0f);
}

void BMPGenerator::MakeBMPTile(int tileX, int tileY, XMINT2 tileSize, int tileSampleCount, ImageBMPData* pRGB)
{
	unique_ptr<HDefaultSampler> sampler = make_unique<HDefaultSampler>(1, 1, false, 4);

	XMINT2 screenSize = { (int)m_dxResources->GetOutputSize().x, (int)m_dxResources->GetOutputSize().y };
	for (int i = 0; i < tilesize.x; i++)
	{
		for (int j = 0; j < tilesize.y; j++)
		{
			XMINT2 pixel(tileX * tilesize.x + i, tileY * tilesize.y + j);

			if (pixel.y >= screenSize.y || pixel.x >= screenSize.x)
				continue;

			unique_ptr<HSampler> tileSampler = sampler->Clone();

			//printf("%d %d\n", x, y);
			tileSampler->GenerateSampleData(pixel);

			XMCOLORV LV = XMVectorZero();
			do
			{
				Ray ray = m_mainCamera->GenerateRay((float)pixel.x, (float)pixel.y);
				WhittedIntegrator whi;
				XMCOLOR3 L = whi.Li(ray, *tileSampler, *this, 0, nullptr);
				//printf("R: %f, G: %f, B: %f\n", L.x, L.y, L.z);
				LV += XMLoadFloat3(&L);
			} while (tileSampler->NextSample());

			XMFLOAT3 result;
			XMStoreFloat3(&result, LV);
			XMINT3 resultRGB(result.x > 1.0f ? 255 : (int)(result.x * 255.0f),
				result.y > 1.0f ? 255 : (int)(result.y * 255.0f),
				result.z > 1.0f ? 255 : (int)(result.z * 255.0f));

			int rgbIdx = (screenSize.y - pixel.y - 1) * screenSize.x + pixel.x;
			pRGB[rgbIdx].r += resultRGB.x;
			pRGB[rgbIdx].g += resultRGB.y;
			pRGB[rgbIdx].b += resultRGB.z;
		}
	}

	m_makingProcessIndex++;
	float percent = ((float)m_makingProcessIndex / (float)tileSampleCount) * 100.0f;
	printf("\r%.2f%%..", percent < 100.0f ? percent : 100.0f);
}
