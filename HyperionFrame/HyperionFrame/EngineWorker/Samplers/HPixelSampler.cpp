#include "HPixelSampler.h"
#include "RandomNumberGenerator.h"

HPixelSampler::HPixelSampler(int64_t samplesPerPixel, HInt nSampledDimensions) :
	HSampler(samplesPerPixel)
{
	for (HInt i = 0; i < nSampledDimensions; i++)
	{
		samples1D.push_back(std::vector<HFloat>((HUInt)samplesPerPixel));
		samples2D.push_back(std::vector<HFloat2>((HUInt)samplesPerPixel));
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

HFloat HPixelSampler::Get1D()
{
	if (current1DDimension < (HInt)samples1D.size())
		return samples1D[current1DDimension++][(HUInt)currentPixelSampleIndex];
	else return randRange01();
}

HFloat2 HPixelSampler::Get2D()
{
	if (current2DDimension < (HInt)samples2D.size())
		return samples2D[current2DDimension++][(HUInt)currentPixelSampleIndex];
	else return HFloat2(randRange01(), randRange01());
}
