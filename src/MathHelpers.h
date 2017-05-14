///////////////////////////////////////////////////////////
//
// Copyright(c) 2017 Rasmus Karlsson
//
///////////////////////////////////////////////////////////

#pragma once
#include <windows.h>
#include <Dwmapi.h>
#include <glm\glm.hpp>

using namespace glm;

class MathHelpers {

public:
	
	static float Lerp(float start, float end, float t);
	static vec2 Lerp(vec2 start, vec2 end, float t);
	static vec3 PixelPosToWorldPos(vec2 screenPos);
	static vec3 PixelPosToWorldPos(float x,float y);
	static int CounterToPowerOfTwo(int c);

};
