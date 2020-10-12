///////////////////////////////////////////////////////////
//
// Copyright(c) 2017 Rasmus Karlsson
//
///////////////////////////////////////////////////////////

#pragma once
#include "MathHelpers.h"

#include <vector>

#include "Renderer.h"

///////////////////////////////////////////////////////////

int MathHelpers::GetNextPowerOfTwo(int value)
{
	int power = 1;
	while (power < value)
		power *= 2;
	return power;
}

float MathHelpers::Lerp(float start, float end, float t)
{
	return (start * (1.0f - t)) + (end * t);
}

vec2 MathHelpers::Lerp(vec2 start, vec2 end, float t)
{
	return (start * (1.0f - t)) + (end * t);
}

vec3 MathHelpers::PixelPosToWorldPos(vec2 screenPos)
{
	vec2 screen = vec2(0.5f*Renderer::GetWidth(), 0.5f*Renderer::GetHeight());
	vec2 screenPosition = vec2(screenPos.x, Renderer::GetHeight() - screenPos.y);
	vec3 worldPos = vec3((screenPosition - screen)/ screen, 0.0f);
	return worldPos;
}

vec3 MathHelpers::PixelPosToWorldPos(float x, float y)
{
	vec2 screen = vec2(0.5f*Renderer::GetWidth(), 0.5f*Renderer::GetHeight());
	vec3 worldPos = vec3((vec2(x, Renderer::GetHeight() - y) - screen) / screen, 0.0f);
	return worldPos;
}

int MathHelpers::CounterToPowerOfTwo(int c)
{
	return (1 << c);
}

void MathHelpers::GetReverseSpiralCoordinates(std::vector<ivec2>& vectors, int width, int height)
{
	/* k - starting row index
	l - starting column index*/
	int i, k = 0, l = 0;

	// Counter for single dimension array 
	//in which elements will be stored 
	int z = 0;

	while (k < width && l < height)
	{
		for (i = l; i < height; ++i)
		{
			vectors[z] = ivec2(k,i);
			++z;
		}
		k++;

		for (i = k; i < width; ++i)
		{
			vectors[z] = ivec2(i, height - 1);
			++z;
		}
		height--;

		if (k < width)
		{
			for (i = height - 1; i >= l; --i)
			{
				vectors[z] = ivec2(width - 1, i);
				++z;
			}
			width--;
		}

		if (l < height)
		{
			for (i = width - 1; i >= k; --i)
			{
				vectors[z] = ivec2(i, l);
				++z;
			}
			l++;
		}
	}
}