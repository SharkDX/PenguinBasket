#pragma once
#include <math.h>
#include <random>

class Noise
{
public:
	Noise(unsigned seed)
	{
		srand(seed);
		for (unsigned i = 0; i < kMaxVertices; ++i)
		{
			r[i] = (rand() % 1001) / 1000.0;
		}
	}

	float Mix(const float &a, const float &b, const float &t)
	{
		return a * (1 - t) + b * t;
	}

	float Smoothstep(const float &a, const float &b, const float &t)
	{
		float tRemapSmoothstep = t * t * (3 - 2 * t);
		return Mix(a, b, tRemapSmoothstep);
	}

	/// Evaluate the noise function at position x
	float eval(const float &x)
	{
		/// Floor
		int xi = (int) x - (x < 0 && x != (int) x);
		float t = x - xi;
		/// Modulo using &
		int xMin = xi & kMaxVerticesMask;
		int xMax = (xMin + 1) & kMaxVerticesMask;
		float re = Smoothstep(r[xMin], r[xMax], t);
		return 0.5f + re / 8.0f;
	}

	static const unsigned kMaxVertices = 256;
	static const unsigned kMaxVerticesMask = kMaxVertices - 1;
	float r[kMaxVertices];
};

