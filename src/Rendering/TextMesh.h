#pragma once
#include <GL/glew.h>

#include <vector>
#include <iostream>
#include "Entity.h"

using namespace std;

const static string s_charMap = " abcdefghijklmnopqrstuvwxyz0123456789,._!/?[](){}<>\\+-*~='\":;";

class TextMesh : public Entity
{
public:
	TextMesh();
	~TextMesh();

	void SetText(string text);

	unsigned int GetStringSize() { return m_stringSize; };

	void CreateMesh();

	void UpdateText(string newText);

private:

	GLuint m_texcoordBufferObject;
	unsigned int m_stringSize;
	string m_textString;
};
