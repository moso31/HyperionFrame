#include "HDefaultSampler.h"

void HDefaultSampler::GenerateSampleData(XMINT2 & pixel)
{
	int nSamples = xPixelSamples * yPixelSamples;
	for (int i = 0; i < samples1D.size(); i++)
	{
		float invNSamples = 1.0f / (float)nSamples;
		for (int j = 0; j < nSamples; ++j)
		{
			float delta = 0.5f;
			samples1D[i][j] = min((j + delta) * invNSamples, 0.999999999999994f);
		}
	}

	for (int i = 0; i < samples2D.size(); i++)
	{
		float invx = 1.0f / (float)xPixelSamples;
		float invy = 1.0f / (float)yPixelSamples;
		for (int jx = 0; jx < xPixelSamples; ++jx)
		{
			for (int jy = 0; jy < yPixelSamples; ++jy)
			{
				float deltax = 0.5f;
				float deltay = 0.5f;
				samples2D[i][jx * yPixelSamples + jy].x = min((jx + deltax) * invx, 0.999999999999994f);
				samples2D[i][jx * yPixelSamples + jy].y = min((jy + deltay) * invy, 0.999999999999994f);
			}
		}
	}
}
