#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>


#include "Entity.h"

#include "RenderingAPI/RenderingResource.h"

using namespace glm;

class Renderer
{
public:

	//Init
	static void Initialize();
	static void PrintError(uint32 line);

	//Render States
	static void SetOverrideRenderMode(bool value);
	static void SetRenderMode(RENGINE::RENDER_MODE renderMode);
	static void SetClearColor(vec4 color) { m_clearColor = color; };
	static void ClearBuffer();
	static void Enable(RENGINE::RENDER_FEATURE feature);
	static void Disable(RENGINE::RENDER_FEATURE feature);
	static void SetDepthFunction(RENGINE::DEPTH_TEST function);
	static void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height);

	//Shaders
	static void SetShader(uint32 shader);
	static std::string LoadShaderFile(std::string shaderPath);
	static void CompileShaders();

	//Textures
	static void SetTextures(Material& material);
	static void BindTexture(uint32 textureID, uint32 slot);
	static void UploadMaterialProperties(Material& material);

	//Meshes
	static void CreateMesh(string name, uint32 vFormat, uint32 vSize, uint32 vCount, void* vData, uint32 &vBO, uint32 &vAO, uint32 &indexBuffer, uint32 iCount, int* iData);

	static uint32 GenerateVertexArray();
	static uint32 GenerateVertexBuffer(uint32 vCount, uint32 vSize, uint32 vFormat, void* vData);
	static void UpdateSubVertexBuffer(uint32 vbo, uint32 offset, uint32 vSize, uint32 vCount, void* vData);
	static uint32 GenerateIndexBuffer(uint32 iCount, int* iData);
	static void SetIndexBufferOnVertexArray(uint32 vao, uint32 index);

	static void DeleteVertexBufferObject(uint32 vbo);
	static void DeleteVertexArrayObject(uint32 vao);
	
	//Drawings
	static void SetViewMatrix(mat4 viewMatrix);
	static void SetProjectionMatrix(mat4 projectionMatrix);
	static void Render(Entity& entity);
	static void RenderFullscreenQuad();

	//Uniforms

	static void UploadUniform1f(const char* name, float value);
	static void UploadUniform1i(const char* name, int value);
	static void UploadUniform4fv(const char* name, float const* value);
	static void UploadUniformMatrix4fv(const char* name, float const* value);
	

	static map<string, uint32> sm_ShaderMap;

	static uint32 m_currentRenderMode;
	static uint32 m_currentShader;
	static uint32 m_fullscreenShader;
	static uint32 m_simpleShader;
	static uint32 m_terrainShader;
	static uint32 m_textShader;
	static uint32 m_whiteShader;
	static uint32 m_skyShader;
	static uint32 m_gaussianShader;
	static uint32 m_showDepthShader;
	static vec4 m_clearColor;
	static vec4 m_wireColor;

	static mat4 m_viewMatrix;
	static mat4 m_projectionMatrix;
	static mat4 m_viewProjectionMatrix;
	
	static mat4 m_invViewMatrix;
	static mat4 m_invProjectionMatrix;

	static bool m_overrideRenderMode;
};