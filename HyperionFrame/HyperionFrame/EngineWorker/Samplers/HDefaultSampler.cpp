#include "HDefaultSampler.h"

HDefaultSampler::HDefaultSampler(HInt xPixelSamples, HInt yPixelSamples, bool jitterSamples, HInt nSampledDimensions) :
	HPixelSampler(xPixelSamples * yPixelSamples, nSampledDimensions),
	xPixelSamples(xPixelSamples),
	yPixelSamples(yPixelSamples),
	jitterSamples(jitterSamples)
{
}

void HDefaultSampler::GenerateSampleData(HInt2 & pixel)
{
	HInt nSamples = xPixelSamples * yPixelSamples;
	for (size_t i = 0; i < samples1D.size(); i++)
	{
		HFloat invNSamples = 1.0f / (HFloat)nSamples;
		for (HInt j = 0; j < nSamples; ++j)
		{
			HFloat delta = 0.5f;
			samples1D[i][j] = min((j + delta) * invNSamples, H_ONE_MINUS_EPSILON);
		}
	}

	for (size_t i = 0; i < samples2D.size(); i++)
	{
		HFloat invx = 1.0f / (HFloat)xPixelSamples;
		HFloat invy = 1.0f / (HFloat)yPixelSamples;
		for (HInt jx = 0; jx < xPixelSamples; ++jx)
		{
			for (HInt jy = 0; jy < yPixelSamples; ++jy)
			{
				HFloat deltax = 0.5f;
				HFloat deltay = 0.5f;
				samples2D[i][jx * yPixelSamples + jy].x = min((jx + deltax) * invx, H_ONE_MINUS_EPSILON);
				samples2D[i][jx * yPixelSamples + jy].y = min((jy + deltay) * invy, H_ONE_MINUS_EPSILON);
			}
		}
	}
}

unique_ptr<HSampler> HDefaultSampler::Clone(HInt seed)
{
	return unique_ptr<HSampler>(new HDefaultSampler(*this));
}
