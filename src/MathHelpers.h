///////////////////////////////////////////////////////////
//
// Copyright(c) 2017 Rasmus Karlsson
//
///////////////////////////////////////////////////////////

#pragma once
#include <glm\glm.hpp>
#include <vector>

using namespace glm;

class MathHelpers {

public:

	static int		GetNextPowerOfTwo(int value);
	static float	Lerp(float start, float end, float t);
	static vec2		Lerp(vec2 start, vec2 end, float t);
	static vec3		PixelPosToWorldPos(vec2 screenPos);
	static vec3		PixelPosToWorldPos(float x,float y);
	static int		CounterToPowerOfTwo(int c);

	static void		GetReverseSpiralCoordinates(std::vector<ivec2>& vectors, int width, int height);

};
