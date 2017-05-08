///////////////////////////////////////////////////////////
//
// Copyright(c) 2017 Rasmus Karlsson
//
///////////////////////////////////////////////////////////

#pragma once
#include "MathHelpers.h"

///////////////////////////////////////////////////////////

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

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
	vec2 screen = vec2(0.5f*SCREEN_WIDTH, 0.5f*SCREEN_HEIGHT);
	vec2 screenPosition = vec2(screenPos.x, SCREEN_HEIGHT - screenPos.y);
	vec3 worldPos = vec3((screenPosition - screen)/ screen, 0.0f);
	return worldPos;
}

vec3 MathHelpers::PixelPosToWorldPos(float x, float y)
{
	vec2 screen = vec2(0.5f*SCREEN_WIDTH, 0.5f*SCREEN_HEIGHT);
	vec3 worldPos = vec3((vec2(x, SCREEN_HEIGHT - y) - screen) / screen, 0.0f);
	return worldPos;
}