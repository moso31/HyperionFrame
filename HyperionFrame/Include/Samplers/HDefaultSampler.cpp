#include "HDefaultSampler.h"

HDefaultSampler::HDefaultSampler(int xPixelSamples, int yPixelSamples, bool jitterSamples, int nSampledDimensions) :
	HPixelSampler(xPixelSamples * yPixelSamples, nSampledDimensions),
	xPixelSamples(xPixelSamples),
	yPixelSamples(yPixelSamples),
	jitterSamples(jitterSamples)
{
}

void HDefaultSampler::GenerateSampleData(XMINT2 & pixel)
{
	int nSamples = xPixelSamples * yPixelSamples;
	for (size_t i = 0; i < samples1D.size(); i++)
	{
		float invNSamples = 1.0f / (float)nSamples;
		for (int j = 0; j < nSamples; ++j)
		{
			float delta = 0.5f;
			samples1D[i][j] = min((j + delta) * invNSamples, ONE_MINUS_EPSILON);
		}
	}

	for (size_t i = 0; i < samples2D.size(); i++)
	{
		float invx = 1.0f / (float)xPixelSamples;
		float invy = 1.0f / (float)yPixelSamples;
		for (int jx = 0; jx < xPixelSamples; ++jx)
		{
			for (int jy = 0; jy < yPixelSamples; ++jy)
			{
				float deltax = 0.5f;
				float deltay = 0.5f;
				samples2D[i][jx * yPixelSamples + jy].x = min((jx + deltax) * invx, ONE_MINUS_EPSILON);
				samples2D[i][jx * yPixelSamples + jy].y = min((jy + deltay) * invy, ONE_MINUS_EPSILON);
			}
		}
	}
}

unique_ptr<HSampler> HDefaultSampler::Clone(int seed)
{
	return unique_ptr<HSampler>(new HDefaultSampler(*this));
}
