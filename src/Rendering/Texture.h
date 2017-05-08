#pragma once
#include <GL/glew.h>

#include <vector>
#include <iostream>

using namespace std;

class Texture
{
public:
	Texture();
	~Texture();

	void CreateFromFile(string filename);
	void CreateFBO(string name, int width, int height);
	void CreateMultipleRenderTargetFrameBuffer(int width, int height);

	unsigned int GetWidth() { return m_textureWidth; };
	unsigned int GetHeight() { return m_textureHeight; };

	vector<unsigned char> GetImageData() { return m_imageData; };

	void SetParameter(GLuint param, GLuint value);
	GLuint GetID() { return m_textureID; };

	void Bind(GLuint slot);

private:
	unsigned int m_textureWidth;
	unsigned int m_textureHeight;

	string m_fileName;

	vector<unsigned char> m_imageData;

	GLuint m_textureID;
};