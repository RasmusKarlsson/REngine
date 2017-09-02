#pragma once

#include <gl\glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.hpp"
#include "Entity.h"


using namespace glm;

class Renderer
{
public:

	static void SetShader(GLuint shader);
	static void ClearBuffer();
	static void SetClearColor(vec4 color) { m_clearColor = color; };
	static void SetRenderStyle(int renderStyle);

	static int m_currentRenderStyle;

	static void Render(Entity* entity, mat4 wvpMatrix);
	static void RenderFullscreenQuad();
	static void CompileShaders();

	static GLuint m_currentShader;
	static GLuint m_fullscreenShader;
	static GLuint m_simpleShader;
	static GLuint m_terrainShader;
	static GLuint m_textShader;
	static GLuint m_whiteShader;
	static GLuint m_skyShader;
	static GLuint m_gaussianShader;
	static GLuint m_showDepthShader;
	static vec4 m_clearColor;

	static vector<GLuint>m_shaders;

private:

};

//Global shader state

