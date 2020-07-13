///////////////////////////////////////////////////////////
//
// Copyright(c) 2017 Rasmus Karlsson
//
///////////////////////////////////////////////////////////

#include "Renderer.h"
#include "Scene.h"
#include "Stats.h"
#include <iostream>

#include <stdio.h>
#include <fstream>

#include "RenderingAPI/RendererContext.h"

extern double timeElapsed;
extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;


uint32 Renderer::m_currentShader = 0;

uint32 Renderer::m_simpleShader = 0;
uint32 Renderer::m_terrainShader = 0;
uint32 Renderer::m_textShader = 0;
uint32 Renderer::m_whiteShader = 0;
uint32 Renderer::m_skyShader = 0;
uint32 Renderer::m_gaussianShader = 0;
uint32 Renderer::m_showDepthShader = 0;

uint32 Renderer::m_fullscreenShader = 0;

uint32 Renderer::m_currentRenderMode = Entity::RENDERSTYLE_STANDARD;
bool Renderer::m_overrideRenderMode = false;

vec4 Renderer::m_clearColor = vec4();
vec4 Renderer::m_wireColor = vec4(1.0,1.0,1.0,1.0);

mat4 Renderer::m_viewMatrix = mat4();
mat4 Renderer::m_projectionMatrix = mat4();
mat4 Renderer::m_viewProjectionMatrix = mat4();

mat4 Renderer::m_invViewMatrix = mat4();
mat4 Renderer::m_invProjectionMatrix = mat4();

map<string, uint32> Renderer::sm_ShaderMap;

void Renderer::Initialize()
{
	RendererContext::Initialize();
}

void Renderer::SetShader(uint32 shader)
{
	if (shader >= 0 && shader != m_currentShader)
	{
		m_currentShader = shader;
		RendererContext::SetShader(m_currentShader);
		Stats::s_shaderBounds++;
	}
}

std::string Renderer::LoadShaderFile(std::string shaderPath)
{
	std::string shaderCode;
	std::ifstream shaderStream(shaderPath.c_str() , std::ios::in);
	if (shaderStream.is_open()) {
		std::string Line = "";
		while (std::getline(shaderStream, Line))
			shaderCode += "\n" + Line;
		shaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory?\n", shaderPath.c_str());
	}
	return shaderCode;
}

void Renderer::SetTextures(Material& material)
{
	if (material.GetDiffuseTexture() != nullptr)
	{
		BindTexture(material.GetDiffuseTexture()->GetTextureID(), 0);
	}

	if (material.GetNormalTexture() != nullptr)
	{
		BindTexture(material.GetNormalTexture()->GetTextureID(),1);
	}

	if (material.GetAmbientTexture() != nullptr)
	{
		BindTexture(material.GetAmbientTexture()->GetTextureID(),2);
	}

	if (material.GetSpecularTexture() != nullptr)
	{
		BindTexture(material.GetSpecularTexture()->GetTextureID(),3);
	}
}

void Renderer::UploadMaterialProperties(Material& material)
{
	SetTextures(material);
	RendererContext::UploadUniform4fv("u_DiffuseColor", value_ptr(material.GetDiffuseColor()));

	if(m_currentRenderMode == RENGINE::WIREFRAME)
	{
		RendererContext::UploadUniform4fv("u_WireColor", value_ptr(m_wireColor));
	}
}

void Renderer::CreateMesh(string name, uint32 vFormat, uint32 vSize, uint32 vCount, void* vData, uint32 &vBO, uint32 &vAO, uint32 &indexBuffer, uint32 iCount, int* iData)
{
	vAO = RendererContext::GenerateVertexArray();
	vBO = RendererContext::GenerateVertexBuffer(vCount, vSize, vFormat, vData);
	indexBuffer = RendererContext::GenerateIndexBuffer(iCount, iData);
}

uint32 Renderer::GenerateVertexArray()
{
	return RendererContext::GenerateVertexArray();
}

uint32 Renderer::GenerateVertexBuffer(uint32 vCount, uint32 vSize, uint32 vFormat, void* vData)
{
	return RendererContext::GenerateVertexBuffer(vCount, vSize, vFormat, vData);
}

void Renderer::UpdateSubVertexBuffer(uint32 vbo, uint32 offset, uint32 vSize, uint32 vCount, void* vData)
{
	RendererContext::UpdateSubVertexBuffer(vbo, offset, vSize, vCount, vData);
}

uint32 Renderer::GenerateIndexBuffer(uint32 iCount, int* iData)
{
	return RendererContext::GenerateIndexBuffer(iCount, iData);
}

void Renderer::SetIndexBufferOnVertexArray(uint32 vao, uint32 index)
{
	RendererContext::SetIndexBufferOnVertexArray(vao, index);
}

void Renderer::DeleteVertexBufferObject(uint32 vbo)
{
	RendererContext::DeleteVertexBufferObject(vbo);
}

void Renderer::DeleteVertexArrayObject(uint32 vao)
{
	RendererContext::DeleteVertexArrayObject(vao);
}

void Renderer::BindTexture(uint32 textureID, uint32 slot)
{
	if (s_boundTextures[slot] != textureID)
	{
		RendererContext::BindTexture(textureID, slot);
		s_boundTextures[slot] = textureID;
		Stats::s_textureBounds++;
	}
}

void Renderer::Render(Entity& entity)
{
	if(entity.GetMaterial())
	{
		SetRenderMode(entity.GetRenderStyle());
		SetShader(entity.GetMaterial()->GetShader());
		UploadMaterialProperties(*entity.GetMaterial());
	}

	mat4 wvpMatrix = m_viewProjectionMatrix * entity.GetWorldMatrix();
	
	RendererContext::UploadUniformMatrix4fv("u_WorldViewProjection", value_ptr(wvpMatrix));
	RendererContext::UploadUniformMatrix4fv("u_World", value_ptr(entity.GetWorldMatrix()));

	RendererContext::UploadUniformMatrix4fv("u_invView", value_ptr(m_invViewMatrix));
	RendererContext::UploadUniformMatrix4fv("u_invProjection", value_ptr(m_invProjectionMatrix));

	RendererContext::UploadUniform1f("u_Time", static_cast<float>(timeElapsed));

	//TODO: Move these
	RendererContext::UploadUniform1i("Sampler0", 0);
	RendererContext::UploadUniform1i("Sampler1", 1);
	RendererContext::UploadUniform1i("Sampler2", 2);
	RendererContext::UploadUniform1i("Sampler3", 3);
	RendererContext::UploadUniform1i("Sampler4", 4);
	
	RendererContext::BindVertexArrayObject(entity.GetVao());
	const uint32 indexSize = entity.GetIndexSize();
	RendererContext::DrawElements(indexSize, RENGINE::POLYGON_TYPE_TRIANGLES, RENGINE::INDEX_TYPE_UNSIGNED_INT);

	Stats::s_vertexCount += entity.GetTriangleCount();
	Stats::s_indexCount += indexSize;
}

void Renderer::RenderFullscreenQuad()
{
	RendererContext::RenderFullscreenQuad();
}

void Renderer::UploadUniform1f(const char* name, float value)
{
	RendererContext::UploadUniform1f(name, value);
}

void Renderer::UploadUniform1i(const char* name, int value)
{
	RendererContext::UploadUniform1i(name, value);
}

void Renderer::UploadUniform4fv(const char* name, float const* value)
{
	RendererContext::UploadUniform4fv(name, value);
}

void Renderer::UploadUniformMatrix4fv(const char* name, float const* value)
{
	RendererContext::UploadUniformMatrix4fv(name, value);
}

void Renderer::CompileShaders()
{
	uint32 simpleVertexShader = RendererContext::CreateVertexShader(LoadShaderFile("src/shaders/simple.vert"));
	uint32 simpleFragmentShader = RendererContext::CreateFragmentShader(LoadShaderFile("src/shaders/simple.frag"));

	uint32 fullscreenVertex = RendererContext::CreateVertexShader(LoadShaderFile("src/shaders/fullscreenPass.vert"));

	m_simpleShader = RendererContext::CreateShaderProgram(
		simpleVertexShader,
		simpleFragmentShader);
	m_terrainShader = RendererContext::CreateShaderProgram(
		RendererContext::CreateVertexShader(LoadShaderFile("src/shaders/splatmap.vert")),
		RendererContext::CreateFragmentShader(LoadShaderFile("src/shaders/splatmap.frag")));
	m_textShader = RendererContext::CreateShaderProgram(
		RendererContext::CreateVertexShader(LoadShaderFile("src/shaders/2dUI.vert")),
		RendererContext::CreateFragmentShader(LoadShaderFile("src/shaders/2dUI.frag")));
	m_whiteShader = RendererContext::CreateShaderProgram(
		simpleVertexShader,
		RendererContext::CreateFragmentShader(LoadShaderFile("src/shaders/whiteColor.frag")));
	m_skyShader = RendererContext::CreateShaderProgram(
		RendererContext::CreateVertexShader(LoadShaderFile("src/shaders/skybox.vert")),
		RendererContext::CreateFragmentShader(LoadShaderFile("src/shaders/skybox.frag")));
	m_fullscreenShader = RendererContext::CreateShaderProgram(
		fullscreenVertex,
		simpleFragmentShader);
	m_gaussianShader = RendererContext::CreateShaderProgram(
		fullscreenVertex,
		RendererContext::CreateFragmentShader(LoadShaderFile("src/shaders/gaussianBlur.frag")));
	m_showDepthShader = RendererContext::CreateShaderProgram(
		fullscreenVertex,
		RendererContext::CreateFragmentShader(LoadShaderFile("src/shaders/showDepth.frag")));
	

	//sm_ShaderMap["Standard"] = LoadShaders("simple.vert", "simple.frag");
}

void Renderer::Enable(RENGINE::RENDER_FEATURE feature)
{
	RendererContext::Enable(feature);
}

void Renderer::Disable(RENGINE::RENDER_FEATURE feature)
{
	RendererContext::Disable(feature);
}

void Renderer::SetDepthFunction(RENGINE::DEPTH_TEST function)
{
	RendererContext::SetDepthFunction(function);
}

void Renderer::SetViewport(uint32 x, uint32 y, uint32 width, uint32 height)
{
	RendererContext::SetViewport(x, y, width, height);
}

void Renderer::PrintError(uint32 line)
{
	RendererContext::PrintError(line);
}

void Renderer::ClearBuffer()
{
	RendererContext::ClearBuffer(m_clearColor);
}

void Renderer::SetRenderMode(RENGINE::RENDER_MODE renderMode)
{
	if (m_overrideRenderMode) return;
	if (m_currentRenderMode == renderMode) return;

	RendererContext::SetRenderMode(renderMode);

	m_currentRenderMode = renderMode;
	Stats::s_renderStyleChanges++;
}

void Renderer::SetOverrideRenderMode(bool value)
{
	m_overrideRenderMode = value;
}

void Renderer::SetViewMatrix(mat4 viewMatrix)
{
	m_viewMatrix = viewMatrix;
	m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;

	m_invViewMatrix = inverse(m_viewMatrix);
}

void Renderer::SetProjectionMatrix(mat4 projectionMatrix)
{
	m_projectionMatrix = projectionMatrix;
	m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
	
	m_invProjectionMatrix = inverse(m_projectionMatrix);
}
