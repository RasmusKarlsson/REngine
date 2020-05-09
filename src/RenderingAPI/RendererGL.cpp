
#if defined (RENGINE_OPENGL)

#include "RenderingAPI/RendererContext.h"

#include <GL/glew.h>
#include <cstdio>

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

//Static variables
uint32 RendererContext::sm_currentShader = 0;

void RendererContext::SetShader(uint32 shader)
{
	sm_currentShader = shader;
	glUseProgram(shader);
}

void RendererContext::BindTexture(uint32 textureID, uint32 slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void RendererContext::ClearBuffer(vec4 clearColor)
{
	glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RendererContext::BindVertexArrayObject(uint32 vao)
{
	glBindVertexArray(vao);
}

void RendererContext::DrawElements(uint32 indexCount, RENGINE_POLYGON_TYPE polyType, RENGINE_INDEX_TYPE indexType)
{
	uint32 glPolyType = GL_TRIANGLES;
	switch (polyType)
	{
	case RENGINE_POLYGON_TYPE_POINTS:			glPolyType = GL_POINTS; break;
	case RENGINE_POLYGON_TYPE_LINES:			glPolyType = GL_LINES; break;
	case RENGINE_POLYGON_TYPE_LINE_LOOP:		glPolyType = GL_LINE_LOOP; break;
	case RENGINE_POLYGON_TYPE_LINE_STRIP:		glPolyType = GL_LINE_STRIP; break;
	case RENGINE_POLYGON_TYPE_TRIANGLES:		glPolyType = GL_TRIANGLES; break;
	case RENGINE_POLYGON_TYPE_TRIANGLE_STRIP:	glPolyType = GL_TRIANGLE_STRIP; break;
	case RENGINE_POLYGON_TYPE_TRIANGLE_FAN:		glPolyType = GL_TRIANGLE_FAN; break;
	case RENGINE_POLYGON_TYPE_QUADS:			glPolyType = GL_QUADS; break;
	case RENGINE_POLYGON_TYPE_QUAD_STRIP:		glPolyType = GL_QUAD_STRIP; break;
	}

	uint32 glIndexType = GL_UNSIGNED_INT;
	switch (indexType)
	{
	case RENGINE_INDEX_TYPE_BYTE:				glIndexType = GL_BYTE; break;
	case RENGINE_INDEX_TYPE_UNSIGNED_BYTE:		glIndexType = GL_UNSIGNED_BYTE; break;
	case RENGINE_INDEX_TYPE_SHORT:				glIndexType = GL_SHORT; break;
	case RENGINE_INDEX_TYPE_UNSIGNED_SHORT:		glIndexType = GL_UNSIGNED_SHORT; break;
	case RENGINE_INDEX_TYPE_INT:				glIndexType = GL_INT; break;
	case RENGINE_INDEX_TYPE_UNSIGNED_INT:		glIndexType = GL_UNSIGNED_INT; break;
	case RENGINE_INDEX_TYPE_FLOAT:				glIndexType = GL_FLOAT; break; //Remove
	}
	glDrawElements(glPolyType, indexCount, glIndexType, nullptr);
	glBindVertexArray(0);
}

void RendererContext::SetRenderMode(RENGINE_RENDER_MODE renderMode)
{
	switch (renderMode)
	{
	case DISABLED:
		break;
	case SOLID:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_DEPTH_TEST);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		break;
	case OPACITY:
		glDisable(GL_DEPTH_TEST);
		break;
	case ADD:
		glDisable(GL_DEPTH_TEST);
		break;
	case UI:
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		break;

	case WIREFRAME:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glPolygonOffset(4.0, 4.0);
		glLineWidth(1.0f);
		break;

	default:
		break;
	}
}

void RendererContext::UploadUniform1f(const char* name, float value)
{
	glUniform1f(glGetUniformLocation(sm_currentShader, name), value);
}

void RendererContext::UploadUniform1i(const char* name, int value)
{
	glUniform1i(glGetUniformLocation(sm_currentShader, name), value);
}

void RendererContext::UploadUniform2fv(const char* name, float const* value)
{
	glUniform2fv(glGetUniformLocation(sm_currentShader, name),1, value);
}

void RendererContext::UploadUniformMatrix4fv(const char* name, float const* value)
{
	glUniformMatrix4fv(glGetUniformLocation(sm_currentShader, name), 1, GL_FALSE, value);
}

uint32 RendererContext::CreateTexture(uint32 width, uint32 height, RENGINE_PIXEL_FORMAT internalPixelFormat, RENGINE_PIXEL_FORMAT pixelFormat, RENGINE_PIXEL_TYPE pixelType, uint8* pixels)
{
	uint32 textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	uint32 glPixelFormat = GL_RGB;
	switch(pixelFormat)
	{
	case RENGINE_PIXEL_FORMAT_RGB: glPixelFormat = GL_RGB; break;
	case RENGINE_PIXEL_FORMAT_RGBA: glPixelFormat = GL_RGBA; break;
	case RENGINE_PIXEL_FORMAT_RGB16F: glPixelFormat = GL_RGB16F; break;
	case RENGINE_PIXEL_FORMAT_RGB32F: glPixelFormat = GL_RGB32F; break;
	case RENGINE_PIXEL_FORMAT_RGBA16F: glPixelFormat = GL_RGBA16F; break;
	case RENGINE_PIXEL_FORMAT_RGBA32F: glPixelFormat = GL_RGBA32F; break;
	}
	
	uint32 glInternalPixelFormat = GL_RGB;
	switch (internalPixelFormat)
	{
	case RENGINE_PIXEL_FORMAT_RGB: glInternalPixelFormat = GL_RGB; break;
	case RENGINE_PIXEL_FORMAT_RGBA: glInternalPixelFormat = GL_RGBA; break;
	case RENGINE_PIXEL_FORMAT_RGB16F: glInternalPixelFormat = GL_RGB16F; break;
	case RENGINE_PIXEL_FORMAT_RGB32F: glInternalPixelFormat = GL_RGB32F; break;
	case RENGINE_PIXEL_FORMAT_RGBA16F: glInternalPixelFormat = GL_RGBA16F; break;
	case RENGINE_PIXEL_FORMAT_RGBA32F: glInternalPixelFormat = GL_RGBA32F; break;
	}

	uint32 glPixelType = GL_UNSIGNED_BYTE;
	switch (pixelType)
	{
	case RENGINE_PIXEL_TYPE_UNSIGNED_BYTE:	glPixelType = GL_UNSIGNED_BYTE; break;
	case RENGINE_PIXEL_TYPE_FLOAT:			glPixelType = GL_FLOAT; break;
	}
	
	glTexImage2D(GL_TEXTURE_2D, 0, glInternalPixelFormat, width, height, 0, glPixelFormat, glPixelType, pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);
	return textureID;
}
#endif