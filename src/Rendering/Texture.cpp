///////////////////////////////////////////////////////////
//
// Copyright(c) 2017 Rasmus Karlsson
//
///////////////////////////////////////////////////////////

#pragma once
#include <lodepng.cpp>
#include "hdrloader.h"
#include "Texture.h"

Texture::Texture()
{
	
}

Texture::~Texture()
{

}

Texture::Texture(string filename)
{
	CreateFromFile(filename);
}


void Texture::CreateFromFile(string filename)
{
	m_fileName = filename;
	unsigned int error = lodepng::decode(m_imageData, m_textureWidth, m_textureHeight, m_fileName.c_str());

	if (error)
	{
		std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << " at file path: " << filename << std::endl;
		return;
	}

	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_textureWidth, m_textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &m_imageData[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::CreateHDRFromFile(string filename)
{
	m_fileName = filename;
	HDRLoaderResult hdrTexture;
	bool result = HDRLoader::load(m_fileName.c_str(), hdrTexture);

	m_textureWidth = hdrTexture.width;
	m_textureHeight = hdrTexture.height;

	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, hdrTexture.width, hdrTexture.height, 0, GL_RGB, GL_FLOAT, hdrTexture.cols);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::CreateFBO(string name, int width, int height)
{
	m_fileName = name;
	m_textureWidth = width;
	m_textureHeight = height;

	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_textureWidth, m_textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
}

void Texture::CreateMultipleRenderTargetFrameBuffer(int width, int height)
{
	m_textureWidth = width;
	m_textureHeight = height;

	GLuint gBuffer;
	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	GLuint gPosition, gNormal, gAlbedoSpec;

	// - Position color buffer
	glGenTextures(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_textureWidth, m_textureHeight, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

	// - Normal color buffer
	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_textureWidth, m_textureHeight, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

	// - Color + Specular color buffer
	glGenTextures(1, &gAlbedoSpec);
	glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_textureWidth, m_textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);

	// - Tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
	GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);
}

void Texture::SetParameter(GLuint param, GLuint value)
{
	glTexParameterf(GL_TEXTURE_2D, param, value);
}

void Texture::Bind(GLuint slot)
{
	glActiveTexture(GL_TEXTURE0+ slot);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
}