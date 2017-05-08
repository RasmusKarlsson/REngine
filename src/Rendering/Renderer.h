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
	static void SetClearColor(vec4 color) {	m_clearColor = color; };

	static void Render(Entity* entity, mat4 wvpMatrix);
	static void CompileShaders();

	static GLuint m_currentShader;
	static GLuint m_simpleShader;
	static GLuint m_terrainShader;
	static GLuint m_textShader;

	static vec4 m_clearColor;

private:


};

//Global shader state

