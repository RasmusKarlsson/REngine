#pragma once

#include <glm/glm.hpp>
#include <string>


#include "RenderingResource.h"

using namespace glm;
typedef unsigned int uint32;

class RendererContext
{
public:
	static void SetShader(uint32 shader);
	static void BindTexture(uint32 textureID, uint32 slot);
	static void ClearBuffer(vec4 clearColor);
	static void BindVertexArrayObject(uint32 vao);
	static void DrawElements(uint32 indexCount, RENGINE_POLYGON_TYPE polyType, RENGINE_INDEX_TYPE indexType);
	static void SetRenderMode(RENGINE_RENDER_MODE renderMode);

	//Uniforms
	static void UploadUniform1f(const char* name, float value);
	static void UploadUniform1i(const char* name, int value);
	static void UploadUniform2fv(const char* name, float const* value);
	static void UploadUniformMatrix4fv(const char* name, float const* value);

	//Textures
	static uint32 CreateTexture(
		uint32 width, 
		uint32 height, 
		RENGINE_PIXEL_FORMAT internalPixelFormat, 
		RENGINE_PIXEL_FORMAT pixelFormat,
		RENGINE_PIXEL_TYPE pixelType,
		uint8* pixels);

	//static vec4 m_clearColor;
	static uint32 sm_currentShader;
};


/*
vec4 RendererContext::m_clearColor = vec4(0.0, 0.0, 0.0, 0.0);

void RendererContext::SetClearColor(vec4 color)
{
	m_clearColor = color;
}

*/