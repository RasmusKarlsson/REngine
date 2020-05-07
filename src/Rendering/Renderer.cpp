///////////////////////////////////////////////////////////
//
// Copyright(c) 2017 Rasmus Karlsson
//
///////////////////////////////////////////////////////////

#include "Renderer.h"
#include "Scene.h"
#include "Stats.h"
#include <iostream>

extern double timeElapsed;
extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

inline int printOglError(int line)
{
	GLenum glErr;
	int    retCode = 0;

	glErr = glGetError();
	if (glErr != GL_NO_ERROR)
	{
		printf("glError in file %d @ %d\n", line, glErr);
		retCode = 1;
	}
	return retCode;
}

GLuint Renderer::m_currentShader = 0;

GLuint Renderer::m_simpleShader = 0;
GLuint Renderer::m_terrainShader = 0;
GLuint Renderer::m_textShader = 0;
GLuint Renderer::m_whiteShader = 0;
GLuint Renderer::m_skyShader = 0;
GLuint Renderer::m_gaussianShader = 0;
GLuint Renderer::m_showDepthShader = 0;

GLuint Renderer::m_fullscreenShader = 0;

int Renderer::m_currentRenderStyle = Entity::RENDERSTYLE_STANDARD;

vec4 Renderer::m_clearColor = vec4();

void Renderer::BindTextures(Material* material)
{

}

void Renderer::SetShader(GLuint shader)
{
	if (shader >= 0 && (int)shader != (int)m_currentShader)
	{
		m_currentShader = shader;
		glUseProgram(m_currentShader);
		Stats::s_shaderBounds++;
	}
}

void Renderer::Render(Entity& entity, mat4 wvpMatrix)
{
	if(entity.GetMaterial())
	{
		entity.GetMaterial()->BindTextures();
		SetShader(entity.GetMaterial()->GetShader());
	}
	
	glDisable(GL_CULL_FACE);
	glUniformMatrix4fv(glGetUniformLocation(m_currentShader, "u_WorldViewProjection"), 1, GL_FALSE, value_ptr(wvpMatrix));
	glUniformMatrix4fv(glGetUniformLocation(m_currentShader, "u_World"), 1, GL_FALSE, value_ptr(entity.GetWorldMatrix()));
	glUniform1f(glGetUniformLocation(m_currentShader, "u_Time"), static_cast<GLfloat>(timeElapsed));
	glUniform1i(glGetUniformLocation(m_currentShader, "Sampler0"), 0);
	glUniform1i(glGetUniformLocation(m_currentShader, "Sampler1"), 1);
	glUniform1i(glGetUniformLocation(m_currentShader, "Sampler2"), 2);
	glUniform1i(glGetUniformLocation(m_currentShader, "Sampler3"), 3);
	glUniform1i(glGetUniformLocation(m_currentShader, "Sampler4"), 4);
	glBindVertexArray(entity.GetVao());

	const GLuint indexSize = entity.GetIndexSize();
	glDrawElements(GL_TRIANGLES, indexSize, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);

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

void Renderer::ClearBuffer()
{
	glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::SetRenderStyle(int renderStyle)
{
	if (m_currentRenderStyle == renderStyle) return;
	switch (renderStyle)
	{
	case Entity::RENDERSTYLE_STANDARD:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_DEPTH_TEST);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		break;
	case Entity::RENDERSTYLE_OPACITY:
		glDisable(GL_DEPTH_TEST);
		break;
	case Entity::RENDERSTYLE_ADD:
		glDisable(GL_DEPTH_TEST);
		break;
	case Entity::RENDERSTYLE_2D:

		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		break;

	case Entity::RENDERSTYLE_STANDARD_WIRE:

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glPolygonOffset(4.0, 4.0);
		glLineWidth(1.0f);

		break;
	}

	m_currentRenderStyle = renderStyle;
	Stats::s_renderStyleChanges++;
}
