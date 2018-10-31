#include "HPixelSampler.h"
#include "RandomNumberGenerator.h"

HPixelSampler::HPixelSampler(int64_t samplesPerPixel, int nSampledDimensions) :
	HSampler(samplesPerPixel)
{
	for (int i = 0; i < nSampledDimensions; i++)
	{
		samples1D.push_back(std::vector<float>((UINT)samplesPerPixel));
		samples2D.push_back(std::vector<XMFLOAT2>((UINT)samplesPerPixel));
	}
}

bool HPixelSampler::NextSample()
{
	current1DDimension = current2DDimension = 0;
	return HSampler::NextSample();
}

bool HPixelSampler::GotoSample(int64_t sampleIndex)
{
	current1DDimension = current2DDimension = 0;
	return HSampler::GotoSample(sampleIndex);
}

float HPixelSampler::Get1D()
{
	if (current1DDimension < (int)samples1D.size())
		return samples1D[current1DDimension++][(UINT)currentPixelSampleIndex];
	else return randRange01();
}

XMFLOAT2 HPixelSampler::Get2D()
{
	if (current2DDimension < (int)samples2D.size())
		return samples2D[current2DDimension++][(UINT)currentPixelSampleIndex];
	else return XMFLOAT2(randRange01(), randRange01());
}
