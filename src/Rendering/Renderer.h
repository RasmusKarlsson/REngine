#pragma once

//#include <gl\glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Entity.h"

#include "RenderingAPI/RenderingResource.h"

using namespace glm;

class Renderer
{
public:

	static void SetShader(uint32 shader);
	static void SetTextures(Material& material);
	static void BindTexture(uint32 textureID, uint32 slot);
	static void ClearBuffer();
	static void SetClearColor(vec4 color) { m_clearColor = color; };
	static void SetRenderMode(RENGINE_RENDER_MODE renderMode);
	static void SetViewMatrix(mat4 viewMatrix);
	static void SetProjectionMatrix(mat4 projectionMatrix);

	static void Render(Entity& entity);
	static void RenderFullscreenQuad();
	static void CompileShaders();

	static int m_currentRenderMode;
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

	static mat4 m_viewMatrix;
	static mat4 m_projectionMatrix;
	static mat4 m_viewProjectionMatrix;
};