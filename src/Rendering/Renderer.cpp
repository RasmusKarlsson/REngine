///////////////////////////////////////////////////////////
//
// Copyright(c) 2017 Rasmus Karlsson
//
///////////////////////////////////////////////////////////

#include "Renderer.h"
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

vec4 Renderer::m_clearColor = vec4();

void Renderer::SetShader(GLuint shader)
{
	if (shader >= 0 && (int)shader != (int)m_currentShader)
	{
		m_currentShader = shader;
		glUseProgram(m_currentShader);
	}
}

void Renderer::Render(Entity* entity, mat4 wvpMatrix)
{
	//SetShader(shader);
	glDisable(GL_CULL_FACE);
	glUniformMatrix4fv(glGetUniformLocation(m_currentShader, "u_WorldViewProjection"), 1, GL_FALSE, value_ptr(wvpMatrix));
	glUniformMatrix4fv(glGetUniformLocation(m_currentShader, "u_World"), 1, GL_FALSE, value_ptr(entity->GetWorldMatrix()));
	glUniform1f(glGetUniformLocation(m_currentShader, "u_Time"), timeElapsed);
	glUniform1i(glGetUniformLocation(m_currentShader, "Sampler0"), 0);
	glUniform1i(glGetUniformLocation(m_currentShader, "Sampler1"), 1);
	glUniform1i(glGetUniformLocation(m_currentShader, "Sampler2"), 2);
	glUniform1i(glGetUniformLocation(m_currentShader, "Sampler3"), 3);
	glUniform1i(glGetUniformLocation(m_currentShader, "Sampler4"), 4);
	glBindVertexArray(entity->GetVao());
	glDrawElements(GL_TRIANGLES, entity->GetSize(), GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);

}

void Renderer::CompileShaders()
{
	Renderer::m_simpleShader = LoadShaders("simple.vert", "simple.frag");
	Renderer::m_terrainShader = LoadShaders("splatmap.vert", "splatmap.frag");
	Renderer::m_textShader = LoadShaders("2dUI.vert", "2dUI.frag");
	vec2 screenSize((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);
	vec2 elementSize(8.0f, 8.0f);
	glUseProgram(Renderer::m_textShader);
	glUniform2fv(glGetUniformLocation(Renderer::m_textShader, "u_ScreenSize"), 1, value_ptr(screenSize));
	glUniform2fv(glGetUniformLocation(Renderer::m_textShader, "u_ElementSize"), 1, value_ptr(elementSize));
}

void Renderer::ClearBuffer()
{
	glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}