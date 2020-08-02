
#if defined (RENGINE_OPENGL)

#include "RenderingAPI/RendererContext.h"

#include <GL/glew.h>
#include <cstdio>

//Static variables
uint32 RendererContext::sm_currentShader = 0;

#define GL_ALL_BUFFERS GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT

void RendererContext::Initialize()
{
	glewExperimental = GL_TRUE;
	glewInit();
}

void RendererContext::PrintError(uint32 line)
{
	GLenum glErr = glGetError();
	if (glErr != GL_NO_ERROR)
	{
		printf("glError in file %d @ %d\n", line, glErr);
	}
}

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
	glClear(GL_ALL_BUFFERS);
}

void RendererContext::BindVertexArrayObject(uint32 vao)
{
	glBindVertexArray(vao);
}

void RendererContext::DrawElements(uint32 indexCount, RENGINE::POLYGON_TYPE polyType, RENGINE::INDEX_TYPE indexType)
{
	uint32 glPolyType = GL_TRIANGLES;
	switch (polyType)
	{
	case RENGINE::POLYGON_TYPE_POINTS:			glPolyType = GL_POINTS; break;
	case RENGINE::POLYGON_TYPE_LINES:			glPolyType = GL_LINES; break;
	case RENGINE::POLYGON_TYPE_LINE_LOOP:		glPolyType = GL_LINE_LOOP; break;
	case RENGINE::POLYGON_TYPE_LINE_STRIP:		glPolyType = GL_LINE_STRIP; break;
	case RENGINE::POLYGON_TYPE_TRIANGLES:		glPolyType = GL_TRIANGLES; break;
	case RENGINE::POLYGON_TYPE_TRIANGLE_STRIP:	glPolyType = GL_TRIANGLE_STRIP; break;
	case RENGINE::POLYGON_TYPE_TRIANGLE_FAN:	glPolyType = GL_TRIANGLE_FAN; break;
	case RENGINE::POLYGON_TYPE_QUADS:			glPolyType = GL_QUADS; break;
	case RENGINE::POLYGON_TYPE_QUAD_STRIP:		glPolyType = GL_QUAD_STRIP; break;
	}

	uint32 glIndexType = GL_UNSIGNED_INT;
	switch (indexType)
	{
	case RENGINE::INDEX_TYPE_BYTE:				glIndexType = GL_BYTE; break;
	case RENGINE::INDEX_TYPE_UNSIGNED_BYTE:		glIndexType = GL_UNSIGNED_BYTE; break;
	case RENGINE::INDEX_TYPE_SHORT:				glIndexType = GL_SHORT; break;
	case RENGINE::INDEX_TYPE_UNSIGNED_SHORT:	glIndexType = GL_UNSIGNED_SHORT; break;
	case RENGINE::INDEX_TYPE_INT:				glIndexType = GL_INT; break;
	case RENGINE::INDEX_TYPE_UNSIGNED_INT:		glIndexType = GL_UNSIGNED_INT; break;
	case RENGINE::INDEX_TYPE_FLOAT:				glIndexType = GL_FLOAT; break; //Remove
	}
	glDrawElements(glPolyType, indexCount, glIndexType, nullptr);
	glBindVertexArray(0);
}

void RendererContext::SetRenderMode(RENGINE::RENDER_MODE renderMode)
{
	switch (renderMode)
	{
	case RENGINE::DISABLED:
		break;
	case RENGINE::SOLID:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_DEPTH_TEST);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		break;
	case RENGINE::OPACITY:
		glDisable(GL_DEPTH_TEST);
		break;
	case RENGINE::ADD:
		glDisable(GL_DEPTH_TEST);
		break;
	case RENGINE::UI:
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		break;

	case RENGINE::WIREFRAME:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glPolygonOffset(4.0, 4.0);
		glLineWidth(1.0f);
		break;

	default:
		break;
	}
}

//TODO: Cache uniform locations
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

void RendererContext::UploadUniform4fv(const char* name, float const* value)
{
	glUniform4fv(glGetUniformLocation(sm_currentShader, name), 1, value);
}

void RendererContext::UploadUniformMatrix4fv(const char* name, float const* value)
{
	glUniformMatrix4fv(glGetUniformLocation(sm_currentShader, name), 1, GL_FALSE, value);
}

uint32 RendererContext::CreateTexture(uint32 width, uint32 height, RENGINE::PIXEL_FORMAT internalPixelFormat, RENGINE::PIXEL_FORMAT pixelFormat, RENGINE::PIXEL_TYPE pixelType, uint8* pixels)
{
	uint32 textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	uint32 glPixelFormat = GL_RGB;
	switch(pixelFormat)
	{
	case RENGINE::PIXEL_FORMAT_RGB: glPixelFormat = GL_RGB; break;
	case RENGINE::PIXEL_FORMAT_RGBA: glPixelFormat = GL_RGBA; break;
	case RENGINE::PIXEL_FORMAT_RGB16F: glPixelFormat = GL_RGB16F; break;
	case RENGINE::PIXEL_FORMAT_RGB32F: glPixelFormat = GL_RGB32F; break;
	case RENGINE::PIXEL_FORMAT_RGBA16F: glPixelFormat = GL_RGBA16F; break;
	case RENGINE::PIXEL_FORMAT_RGBA32F: glPixelFormat = GL_RGBA32F; break;
	}
	
	uint32 glInternalPixelFormat = GL_RGB;
	switch (internalPixelFormat)
	{
	case RENGINE::PIXEL_FORMAT_RGB: glInternalPixelFormat = GL_RGB; break;
	case RENGINE::PIXEL_FORMAT_RGBA: glInternalPixelFormat = GL_RGBA; break;
	case RENGINE::PIXEL_FORMAT_RGB16F: glInternalPixelFormat = GL_RGB16F; break;
	case RENGINE::PIXEL_FORMAT_RGB32F: glInternalPixelFormat = GL_RGB32F; break;
	case RENGINE::PIXEL_FORMAT_RGBA16F: glInternalPixelFormat = GL_RGBA16F; break;
	case RENGINE::PIXEL_FORMAT_RGBA32F: glInternalPixelFormat = GL_RGBA32F; break;
	}

	uint32 glPixelType = GL_UNSIGNED_BYTE;
	switch (pixelType)
	{
	case RENGINE::PIXEL_TYPE_UNSIGNED_BYTE:	glPixelType = GL_UNSIGNED_BYTE; break;
	case RENGINE::PIXEL_TYPE_FLOAT:			glPixelType = GL_FLOAT; break;
	}
	
	glTexImage2D(GL_TEXTURE_2D, 0, glInternalPixelFormat, width, height, 0, glPixelFormat, glPixelType, pixels);
	return textureID;
}

void RendererContext::GenerateMipmaps()
{
	//TODO: Input the TEXTURE ID and bind before generating.
	glGenerateMipmap(GL_TEXTURE_2D);
}

void RendererContext::SetTextureParameteri(RENGINE::TEXTURE_TYPE texType, RENGINE::TEXTURE_PNAME paramName, RENGINE::TEXTURE_PARAM paramValue)
{
	uint32 glTexType = GL_TEXTURE_1D;
	switch (texType)
	{
	case RENGINE::TEXTURE_TYPE_TEXTURE_1D:						glTexType = GL_TEXTURE_1D; break;
	case RENGINE::TEXTURE_TYPE_TEXTURE_1D_ARRAY:				glTexType = GL_TEXTURE_1D_ARRAY; break;
	case RENGINE::TEXTURE_TYPE_TEXTURE_2D:						glTexType = GL_TEXTURE_2D; break;
	case RENGINE::TEXTURE_TYPE_TEXTURE_2D_ARRAY:				glTexType = GL_TEXTURE_2D_ARRAY; break;
	case RENGINE::TEXTURE_TYPE_TEXTURE_2D_MULTISAMPLE:			glTexType = GL_TEXTURE_2D_MULTISAMPLE; break;
	case RENGINE::TEXTURE_TYPE_TEXTURE_2D_MULTISAMPLE_ARRAY:	glTexType = GL_TEXTURE_2D_MULTISAMPLE_ARRAY; break;
	case RENGINE::TEXTURE_TYPE_TEXTURE_3D:						glTexType = GL_TEXTURE_3D; break;
	case RENGINE::TEXTURE_TYPE_TEXTURE_CUBE_MAP:				glTexType = GL_TEXTURE_CUBE_MAP; break;
	case RENGINE::TEXTURE_TYPE_TEXTURE_CUBE_MAP_ARRAY:			glTexType = GL_TEXTURE_CUBE_MAP_ARRAY; break;
	case RENGINE::TEXTURE_TYPE_TEXTURE_RECTANGLE:				glTexType = GL_TEXTURE_RECTANGLE; break;
	}

	uint32 glParamName = GL_TEXTURE_MIN_FILTER;
	switch (paramName)
	{
	case RENGINE::TEXTURE_PNAME_DEPTH_STENCIL_TEXTURE_MODE:	glParamName = GL_DEPTH_STENCIL_TEXTURE_MODE; break;
	case RENGINE::TEXTURE_PNAME_TEXTURE_BASE_LEVEL:			glParamName = GL_TEXTURE_BASE_LEVEL; break;
	case RENGINE::TEXTURE_PNAME_TEXTURE_COMPARE_FUNC:		glParamName = GL_TEXTURE_COMPARE_FUNC; break;
	case RENGINE::TEXTURE_PNAME_TEXTURE_COMPARE_MODE:		glParamName = GL_TEXTURE_COMPARE_MODE; break;
	case RENGINE::TEXTURE_PNAME_TEXTURE_LOD_BIAS:			glParamName = GL_TEXTURE_LOD_BIAS; break;
	case RENGINE::TEXTURE_PNAME_TEXTURE_MIN_FILTER:			glParamName = GL_TEXTURE_MIN_FILTER; break;
	case RENGINE::TEXTURE_PNAME_TEXTURE_MAG_FILTER:			glParamName = GL_TEXTURE_MAG_FILTER; break;
	case RENGINE::TEXTURE_PNAME_TEXTURE_MIN_LOD:			glParamName = GL_TEXTURE_MIN_LOD; break;
	case RENGINE::TEXTURE_PNAME_TEXTURE_MAX_LOD:			glParamName = GL_TEXTURE_MAX_LOD; break;
	case RENGINE::TEXTURE_PNAME_TEXTURE_MAX_LEVEL:			glParamName = GL_TEXTURE_MAX_LEVEL; break;
	case RENGINE::TEXTURE_PNAME_TEXTURE_SWIZZLE_R:			glParamName = GL_TEXTURE_SWIZZLE_R; break;
	case RENGINE::TEXTURE_PNAME_TEXTURE_SWIZZLE_G:			glParamName = GL_TEXTURE_SWIZZLE_G; break;
	case RENGINE::TEXTURE_PNAME_TEXTURE_SWIZZLE_B:			glParamName = GL_TEXTURE_SWIZZLE_B; break;
	case RENGINE::TEXTURE_PNAME_TEXTURE_SWIZZLE_A:			glParamName = GL_TEXTURE_SWIZZLE_A; break;
	case RENGINE::TEXTURE_PNAME_TEXTURE_WRAP_S:				glParamName = GL_TEXTURE_WRAP_S; break;
	case RENGINE::TEXTURE_PNAME_TEXTURE_WRAP_T:				glParamName = GL_TEXTURE_WRAP_T; break;
	case RENGINE::TEXTURE_PNAME_TEXTURE_WRAP_R:				glParamName = GL_TEXTURE_WRAP_R; break;
	}

	uint32 glParamValue = GL_NEAREST;
	switch (paramValue)
	{
	case RENGINE::TEXTURE_PARAM_NEAREST:					glParamValue = GL_NEAREST; break;
	case RENGINE::TEXTURE_PARAM_LINEAR:						glParamValue = GL_LINEAR; break;
	case RENGINE::TEXTURE_PARAM_NEAREST_MIPMAP_NEAREST:		glParamValue = GL_NEAREST_MIPMAP_NEAREST; break;
	case RENGINE::TEXTURE_PARAM_LINEAR_MIPMAP_NEAREST:		glParamValue = GL_LINEAR_MIPMAP_NEAREST; break;
	case RENGINE::TEXTURE_PARAM_NEAREST_MIPMAP_LINEAR:		glParamValue = GL_NEAREST_MIPMAP_LINEAR; break;
	case RENGINE::TEXTURE_PARAM_LINEAR_MIPMAP_LINEAR:		glParamValue = GL_LINEAR_MIPMAP_LINEAR; break;
	case RENGINE::TEXTURE_PARAM_TEXTURE_MAG_FILTER:			glParamValue = GL_TEXTURE_MAG_FILTER; break;
	case RENGINE::TEXTURE_PARAM_TEXTURE_MIN_FILTER:			glParamValue = GL_TEXTURE_MIN_FILTER; break;
	case RENGINE::TEXTURE_PARAM_TEXTURE_WRAP_S:				glParamValue = GL_TEXTURE_WRAP_S; break;
	case RENGINE::TEXTURE_PARAM_TEXTURE_WRAP_T:				glParamValue = GL_TEXTURE_WRAP_T; break;
	case RENGINE::TEXTURE_PARAM_CLAMP:						glParamValue = GL_CLAMP; break;
	case RENGINE::TEXTURE_PARAM_REPEAT:						glParamValue = GL_REPEAT; break;
	}

	glTexParameteri(glTexType, glParamName, glParamValue);
}

void RendererContext::Enable(RENGINE::RENDER_FEATURE feature)
{
	uint32 glFeature = GL_NEAREST;
	switch (feature)
	{
	case RENGINE::RENDER_FEATURE_DITHER: 								glFeature = GL_DITHER; break;
	case RENGINE::RENDER_FEATURE_MULTISAMPLE: 							glFeature = GL_MULTISAMPLE; break;
	case RENGINE::RENDER_FEATURE_BLEND: 								glFeature = GL_BLEND; break;
	case RENGINE::RENDER_FEATURE_CULL_FACE: 							glFeature = GL_CULL_FACE; break;
	case RENGINE::RENDER_FEATURE_DEBUG_OUTPUT: 							glFeature = GL_DEBUG_OUTPUT; break;
	case RENGINE::RENDER_FEATURE_DEBUG_OUTPUT_SYNCHRONOUS: 				glFeature = GL_DEBUG_OUTPUT_SYNCHRONOUS; break;
	case RENGINE::RENDER_FEATURE_DEPTH_CLAMP: 							glFeature = GL_DEPTH_CLAMP; break;
	case RENGINE::RENDER_FEATURE_DEPTH_TEST: 							glFeature = GL_DEPTH_TEST; break;
	case RENGINE::RENDER_FEATURE_FRAMEBUFFER_SRGB: 						glFeature = GL_FRAMEBUFFER_SRGB; break;
	case RENGINE::RENDER_FEATURE_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING: glFeature = GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING; break;
	case RENGINE::RENDER_FEATURE_SRGB: 									glFeature = GL_SRGB; break;
	case RENGINE::RENDER_FEATURE_LINE_SMOOTH: 							glFeature = GL_LINE_SMOOTH; break;
	case RENGINE::RENDER_FEATURE_POLYGON_OFFSET_FILL: 					glFeature = GL_POLYGON_OFFSET_FILL; break;
	case RENGINE::RENDER_FEATURE_FILL: 									glFeature = GL_FILL; break;
	case RENGINE::RENDER_FEATURE_POLYGON_OFFSET_LINE: 					glFeature = GL_POLYGON_OFFSET_LINE; break;
	case RENGINE::RENDER_FEATURE_LINE: 									glFeature = GL_LINE; break;
	case RENGINE::RENDER_FEATURE_POLYGON_OFFSET_POINT: 					glFeature = GL_POLYGON_OFFSET_POINT; break;
	case RENGINE::RENDER_FEATURE_POINT: 								glFeature = GL_POINT; break;
	case RENGINE::RENDER_FEATURE_POLYGON_SMOOTH: 						glFeature = GL_POLYGON_SMOOTH; break;
	case RENGINE::RENDER_FEATURE_PRIMITIVE_RESTART: 					glFeature = GL_PRIMITIVE_RESTART; break;
	case RENGINE::RENDER_FEATURE_PRIMITIVE_RESTART_FIXED_INDEX: 		glFeature = GL_PRIMITIVE_RESTART_FIXED_INDEX; break;
	case RENGINE::RENDER_FEATURE_MIN_SAMPLE_SHADING_VALUE: 				glFeature = GL_MIN_SAMPLE_SHADING_VALUE; break;
	case RENGINE::RENDER_FEATURE_STENCIL_TEST: 							glFeature = GL_STENCIL_TEST; break;
	case RENGINE::RENDER_FEATURE_TEXTURE_CUBE_MAP_SEAMLESS: 			glFeature = GL_TEXTURE_CUBE_MAP_SEAMLESS; break;
	}
	
	glEnable(glFeature);
}

void RendererContext::Disable(RENGINE::RENDER_FEATURE feature)
{
	uint32 glFeature = GL_NEAREST;
	switch (feature)
	{
	case RENGINE::RENDER_FEATURE_DITHER: 								glFeature = GL_DITHER; break;
	case RENGINE::RENDER_FEATURE_MULTISAMPLE: 							glFeature = GL_MULTISAMPLE; break;
	case RENGINE::RENDER_FEATURE_BLEND: 								glFeature = GL_BLEND; break;
	case RENGINE::RENDER_FEATURE_CULL_FACE: 							glFeature = GL_CULL_FACE; break;
	case RENGINE::RENDER_FEATURE_DEBUG_OUTPUT: 							glFeature = GL_DEBUG_OUTPUT; break;
	case RENGINE::RENDER_FEATURE_DEBUG_OUTPUT_SYNCHRONOUS: 				glFeature = GL_DEBUG_OUTPUT_SYNCHRONOUS; break;
	case RENGINE::RENDER_FEATURE_DEPTH_CLAMP: 							glFeature = GL_DEPTH_CLAMP; break;
	case RENGINE::RENDER_FEATURE_DEPTH_TEST: 							glFeature = GL_DEPTH_TEST; break;
	case RENGINE::RENDER_FEATURE_FRAMEBUFFER_SRGB: 						glFeature = GL_FRAMEBUFFER_SRGB; break;
	case RENGINE::RENDER_FEATURE_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING: glFeature = GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING; break;
	case RENGINE::RENDER_FEATURE_SRGB: 									glFeature = GL_SRGB; break;
	case RENGINE::RENDER_FEATURE_LINE_SMOOTH: 							glFeature = GL_LINE_SMOOTH; break;
	case RENGINE::RENDER_FEATURE_POLYGON_OFFSET_FILL: 					glFeature = GL_POLYGON_OFFSET_FILL; break;
	case RENGINE::RENDER_FEATURE_FILL: 									glFeature = GL_FILL; break;
	case RENGINE::RENDER_FEATURE_POLYGON_OFFSET_LINE: 					glFeature = GL_POLYGON_OFFSET_LINE; break;
	case RENGINE::RENDER_FEATURE_LINE: 									glFeature = GL_LINE; break;
	case RENGINE::RENDER_FEATURE_POLYGON_OFFSET_POINT: 					glFeature = GL_POLYGON_OFFSET_POINT; break;
	case RENGINE::RENDER_FEATURE_POINT: 								glFeature = GL_POINT; break;
	case RENGINE::RENDER_FEATURE_POLYGON_SMOOTH: 						glFeature = GL_POLYGON_SMOOTH; break;
	case RENGINE::RENDER_FEATURE_PRIMITIVE_RESTART: 					glFeature = GL_PRIMITIVE_RESTART; break;
	case RENGINE::RENDER_FEATURE_PRIMITIVE_RESTART_FIXED_INDEX: 		glFeature = GL_PRIMITIVE_RESTART_FIXED_INDEX; break;
	case RENGINE::RENDER_FEATURE_MIN_SAMPLE_SHADING_VALUE: 				glFeature = GL_MIN_SAMPLE_SHADING_VALUE; break;
	case RENGINE::RENDER_FEATURE_SCISSOR_TEST: 							glFeature = GL_SCISSOR_TEST; break;
	case RENGINE::RENDER_FEATURE_STENCIL_TEST: 							glFeature = GL_STENCIL_TEST; break;
	case RENGINE::RENDER_FEATURE_TEXTURE_CUBE_MAP_SEAMLESS: 			glFeature = GL_TEXTURE_CUBE_MAP_SEAMLESS; break;
	}

	glDisable(glFeature);
}

void RendererContext::SetDepthFunction(RENGINE::DEPTH_TEST function)
{
	uint32 glFunction = GL_NEVER;
	switch (function)
	{
	case RENGINE::DEPTH_TEST_NEVER: 			 glFunction = GL_NEVER; break;
	case RENGINE::DEPTH_TEST_ALWAYS: 			 glFunction = GL_ALWAYS; break;
	case RENGINE::DEPTH_TEST_LESS: 				 glFunction = GL_LESS; break;
	case RENGINE::DEPTH_TEST_LEQUAL: 			 glFunction = GL_LEQUAL; break;
	case RENGINE::DEPTH_TEST_GREATER: 			 glFunction = GL_GREATER; break;
	case RENGINE::DEPTH_TEST_GEQUAL: 			 glFunction = GL_GEQUAL; break;
	case RENGINE::DEPTH_TEST_EQUAL: 			 glFunction = GL_EQUAL; break;
	case RENGINE::DEPTH_TEST_NOTEQUAL: 			 glFunction = GL_NOTEQUAL; break;
	}
	glDepthFunc(glFunction);
}

void RendererContext::SetViewport(uint32 x, uint32 y, uint32 width, uint32 height)
{
	glViewport(x, y, width, height);
}

uint32 RendererContext::GenerateVertexArray()
{
	uint32 vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	return vao;
}

uint32 RendererContext::GenerateVertexBuffer(uint32 vertexCount, uint32 vertexSize, uint32 vertexFormat, void* vertexData)
{
	uint32 vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * vertexSize, vertexData, GL_STATIC_DRAW);

	SetVertexAttributePointers(vertexFormat, vertexSize);
	
	return vbo;
}

void RendererContext::UpdateSubVertexBuffer(uint32 vbo, uint32 offset, uint32 vSize, uint32 vCount, void* vData)
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, offset, vSize * vCount, vData);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

uint32 RendererContext::GenerateIndexBuffer(uint32 indexCount, int* indexData)
{
	uint32 indexBuffer;
	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(GLint), indexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return indexBuffer;
}

void RendererContext::SetIndexBufferOnVertexArray(uint32 vao, uint32 index)
{
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
}

void RendererContext::DeleteVertexBufferObject(uint32 vbo)
{
	if(vbo)
		glDeleteBuffers(1, &vbo);
}

void RendererContext::DeleteVertexArrayObject(uint32 vao)
{
	if (vao)
		glDeleteVertexArrays(1, &vao);
}

void RendererContext::SetVertexAttributePointers(uint32 vertexFormat, uint32 vertexSize)
{
	uint32 vertexOffset = 0;

	if(vertexFormat & RENGINE::VA::POSITION)
	{
		glVertexAttribPointer(RENGINE::VA_POINTERS::POSITION_POINTER, 3, GL_FLOAT, GL_FALSE, vertexSize, reinterpret_cast<char *>(vertexOffset));
		vertexOffset += 3 * sizeof(GLfloat);
		glEnableVertexAttribArray(RENGINE::VA_POINTERS::POSITION_POINTER);
	}
	
	if (vertexFormat & RENGINE::VA::NORMAL)
	{
		glVertexAttribPointer(RENGINE::VA_POINTERS::NORMAL_POINTER, 3, GL_FLOAT, GL_FALSE, vertexSize, reinterpret_cast<char *>(vertexOffset));
		vertexOffset += 3 * sizeof(GLfloat);
		glEnableVertexAttribArray(RENGINE::VA_POINTERS::NORMAL_POINTER);
	}
	
	if (vertexFormat & RENGINE::VA::TEXCOORD)
	{
		glVertexAttribPointer(RENGINE::VA_POINTERS::TEXCOORD_POINTER, 2, GL_FLOAT, GL_FALSE, vertexSize, reinterpret_cast<char *>(vertexOffset));
		vertexOffset += 2 * sizeof(GLfloat);
		glEnableVertexAttribArray(RENGINE::VA_POINTERS::TEXCOORD_POINTER);
	}
	
	if (vertexFormat & RENGINE::VA::COLOR)
	{
		glVertexAttribPointer(RENGINE::VA_POINTERS::COLOR_POINTER, 3, GL_FLOAT, GL_FALSE, vertexSize, reinterpret_cast<char *>(vertexOffset));
		vertexOffset += 3 * sizeof(GLfloat);
		glEnableVertexAttribArray(RENGINE::VA_POINTERS::COLOR_POINTER);
	}
}

void RendererContext::RenderFullscreenQuad()
{
	glBindVertexArray(0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

uint32 RendererContext::CreateVertexShader(std::string shaderString)
{
	const uint32 vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	const GLchar *sz = shaderString.c_str();

	glShaderSource(vertexShaderID, 1, (const GLchar**)&sz, NULL);
	glCompileShader(vertexShaderID);

	int iResult = 0;
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &iResult);

	if (!iResult)
	{
		int iLogSize = 0;
		glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &iLogSize);
		if (iLogSize)
		{
			char infoLogChar[4096];
			int infologLength = 0;
			glGetShaderInfoLog(vertexShaderID, 4096, &infologLength, infoLogChar);
			printf("%s", infoLogChar);
		}
	}
	return vertexShaderID;
}

uint32 RendererContext::CreateFragmentShader(std::string shaderString)
{
	const uint32 fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar *sz = shaderString.c_str();

	glShaderSource(fragmentShaderID, 1, (const GLchar**)&sz, NULL);
	glCompileShader(fragmentShaderID);

	int iResult = 0;
	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &iResult);

	if (!iResult)
	{
		int iLogSize = 0;
		glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &iLogSize);
		if (iLogSize)
		{
			char infoLogChar[4096];
			int infologLength = 0;
			glGetShaderInfoLog(fragmentShaderID, 4096, &infologLength, infoLogChar);
			printf("%s", infoLogChar);
		}
	}
	return fragmentShaderID;
}

uint32 RendererContext::CreateShaderProgram(uint32 vertexShader, uint32 fragmentShader)
{
	uint32 programID = glCreateProgram();
	glAttachShader(programID, vertexShader);
	glAttachShader(programID, fragmentShader);

	//Bind Vertex Attibute locations
	glBindAttribLocation(programID, 0, "a_Position");
	glBindAttribLocation(programID, 1, "a_Normal");
	glBindAttribLocation(programID, 2, "a_Texcoord");
	glBindAttribLocation(programID, 3, "a_Color");
	glBindAttribLocation(programID, 4, "a_DataIndex");

	glLinkProgram(programID);

	// Check the program
	int iResult = 0;
	glGetProgramiv(programID, GL_LINK_STATUS, &iResult);
	int iLogSize = 0;
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &iLogSize);
	if(iLogSize)
	{
		char infoLogChar[4096];
		int infologLength = 0;
		glGetShaderInfoLog(programID, 4096, &infologLength, infoLogChar);
		printf("%s", infoLogChar);
	}

	GLint count;

	GLint size; // size of the variable
	GLenum type; // type of the variable (float, vec3 or mat4, etc)

	const GLsizei bufSize = 32; // maximum name length
	GLchar name[bufSize]; // variable name in GLSL
	GLsizei length; // name length

	glUseProgram(programID);
	glGetProgramiv(programID, GL_ACTIVE_UNIFORMS, &count);
	printf("Active Uniforms: %d\n", count);

	GLint sampler2DCount = 0;

	for (int i = 0; i < count; i++)
	{
		glGetActiveUniform(programID, (uint32)i, bufSize, &length, &size, &type, name);

		switch (type)
		{
		case GL_SAMPLER_2D:
			glUniform1i(glGetUniformLocation(programID, name), sampler2DCount++);
			break;
		default:
			break;
		}

		printf("Uniform #%d Type: %u Name: %s\n", i, type, name);
	}
	
	return programID;
}
#endif