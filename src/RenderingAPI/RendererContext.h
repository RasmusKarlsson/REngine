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
	static void DrawElements(uint32 indexCount, RENGINE::POLYGON_TYPE polyType, RENGINE::INDEX_TYPE indexType);
	static void SetRenderMode(RENGINE::RENDER_MODE renderMode);

	//Uniforms
	static void UploadUniform1f(const char* name, float value);
	static void UploadUniform1i(const char* name, int value);
	static void UploadUniform2fv(const char* name, float const* value);
	static void UploadUniform4fv(const char* name, float const* value);
	static void UploadUniformMatrix4fv(const char* name, float const* value);

	//Textures
	static uint32 CreateTexture(
		uint32 width, 
		uint32 height, 
		RENGINE::PIXEL_FORMAT internalPixelFormat, 
		RENGINE::PIXEL_FORMAT pixelFormat,
		RENGINE::PIXEL_TYPE pixelType,
		uint8* pixels);

	static void SetTextureParameteri(RENGINE::TEXTURE_TYPE texType, RENGINE::TEXTURE_PNAME paramName, RENGINE::TEXTURE_PARAM paramValue);
	static void GenerateMipmaps();

	//Render States
	static void Enable(RENGINE::RENDER_FEATURE feature);
	static void Disable(RENGINE::RENDER_FEATURE feature);

	static void SetDepthFunction(RENGINE::DEPTH_TEST function);

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