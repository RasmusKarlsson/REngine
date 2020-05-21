///////////////////////////////////////////////////////////
//
// Copyright(c) 2017 Rasmus Karlsson
//
///////////////////////////////////////////////////////////

#include "Renderer.h"
#include "Scene.h"
#include "Stats.h"
#include <iostream>

#include "Shader.hpp"

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

void Renderer::SetShader(uint32 shader)
{
	if (shader >= 0 && shader != m_currentShader)
	{
		m_currentShader = shader;
		RendererContext::SetShader(m_currentShader);
		Stats::s_shaderBounds++;
	}
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

void Renderer::BindTexture(uint32 textureID, GLuint slot)
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

	RendererContext::UploadUniform1f("u_Time", static_cast<GLfloat>(timeElapsed));

	//TODO: Move these
	RendererContext::UploadUniform1i("Sampler0", 0);
	RendererContext::UploadUniform1i("Sampler1", 1);
	RendererContext::UploadUniform1i("Sampler2", 2);
	RendererContext::UploadUniform1i("Sampler3", 3);
	RendererContext::UploadUniform1i("Sampler4", 4);
	
	RendererContext::BindVertexArrayObject(entity.GetVao());
	const GLuint indexSize = entity.GetIndexSize();
	RendererContext::DrawElements(indexSize, RENGINE::POLYGON_TYPE_TRIANGLES, RENGINE::INDEX_TYPE_UNSIGNED_INT);

	Stats::s_vertexCount += entity.GetTriangleCount();
	Stats::s_indexCount += indexSize;
}

void Renderer::RenderFullscreenQuad()
{
	glBindVertexArray(0);
	glDrawArrays(GL_TRIANGLES, 0, 4);
}

void Renderer::CompileShaders()
{
	m_simpleShader = LoadShaders("simple.vert", "simple.frag");
	m_terrainShader = LoadShaders("splatmap.vert", "splatmap.frag");
	m_textShader = LoadShaders("2dUI.vert", "2dUI.frag");
	m_whiteShader = LoadShaders("simple.vert", "whiteColor.frag");
	m_skyShader = LoadShaders("skybox.vert", "skybox.frag");

	m_fullscreenShader = LoadShaders("fullscreenPass.vert", "simple.frag");
	m_gaussianShader = LoadShaders("fullscreenPass.vert", "gaussianBlur.frag");
	m_showDepthShader = LoadShaders("fullscreenPass.vert", "showDepth.frag");

	vec2 screenSize((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);
	vec2 elementSize(8.0f, 8.0f);
	glUseProgram(m_textShader);
	glUniform2fv(glGetUniformLocation(m_textShader, "u_ScreenSize"), 1, value_ptr(screenSize));
	glUniform2fv(glGetUniformLocation(m_textShader, "u_ElementSize"), 1, value_ptr(elementSize));
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
}

void Renderer::SetProjectionMatrix(mat4 projectionMatrix)
{
	m_projectionMatrix = projectionMatrix;
	m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
}
