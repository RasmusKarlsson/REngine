///////////////////////////////////////////////////////////
//
// Copyright(c) 2017 Rasmus Karlsson
//
///////////////////////////////////////////////////////////

#pragma once
#include "TextMesh.h"
#include <algorithm>

TextMesh::TextMesh()
{
	
}

TextMesh::~TextMesh()
{

}

void TextMesh::SetText(string text)
{
	m_textString = text;
	std::transform(m_textString.begin(), m_textString.end(), m_textString.begin(), ::tolower);
	m_stringSize = m_textString.size();
}

void TextMesh::CreateMesh()
{
	int stringLength = m_textString.size();
	vector<GLfloat> meshVertices;
	vector<GLfloat> meshTexcoords;
	vector<GLfloat> meshNormals;
	vector<GLint> meshIndices;

	for (int i = 0; i < stringLength; ++i) {
		meshVertices.push_back(2.0f*(GLfloat)i);
		meshVertices.push_back(1.0f);
		meshVertices.push_back(0.0f);

		meshVertices.push_back(2.0f*(GLfloat)i);
		meshVertices.push_back(-1.0f);
		meshVertices.push_back(0.0f);

		meshVertices.push_back(2.0f*((GLfloat)i + 1.0f));
		meshVertices.push_back(-1.0f);
		meshVertices.push_back(0.0f);

		meshVertices.push_back(2.0f*((GLfloat)i + 1.0f));
		meshVertices.push_back(1.0f);
		meshVertices.push_back(0.0f);

		int found = s_charMap.find_first_of(m_textString[i]);
		if (found == string::npos)
		{
			found = 0;
		}

		float leftUV = (float)found / 64.0f;
		float rightUV = ((float)found + 1.0f) / 64.0f;

		meshTexcoords.push_back(leftUV);
		meshTexcoords.push_back(0.0f);

		meshTexcoords.push_back(leftUV);
		meshTexcoords.push_back(1.0f);

		meshTexcoords.push_back(rightUV);
		meshTexcoords.push_back(1.0f);

		meshTexcoords.push_back(rightUV);
		meshTexcoords.push_back(0.0f);

		meshIndices.push_back(4*i);
		meshIndices.push_back(4 * i + 1);
		meshIndices.push_back(4 * i + 2);

		meshIndices.push_back(4 * i + 2);
		meshIndices.push_back(4 * i + 3);
		meshIndices.push_back(4 * i);

		meshNormals.push_back(0.0f);
		meshNormals.push_back(0.0f);
		meshNormals.push_back(1.0f);

		meshNormals.push_back(0.0f);
		meshNormals.push_back(0.0f);
		meshNormals.push_back(1.0f);

		meshNormals.push_back(0.0f);
		meshNormals.push_back(0.0f);
		meshNormals.push_back(1.0f);

		meshNormals.push_back(0.0f);
		meshNormals.push_back(0.0f);
		meshNormals.push_back(1.0f);
	}

	GLuint dataBufferObject;
	GLuint normalBufferObject;

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	//Vertex Buffer
	glGenBuffers(1, &m_vboVertex);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboVertex);
	glBufferData(GL_ARRAY_BUFFER, meshVertices.size() * sizeof(GLfloat), meshVertices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//Vertex Buffer
	glGenBuffers(1, &normalBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferObject);
	glBufferData(GL_ARRAY_BUFFER, meshNormals.size() * sizeof(GLfloat), meshNormals.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//Vertex Texcoord Buffer
	glGenBuffers(1, &m_texcoordBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, m_texcoordBufferObject);
	glBufferData(GL_ARRAY_BUFFER, meshTexcoords.size() * sizeof(GLfloat), meshTexcoords.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	/*
	//Vertex Texcoord Buffer
	glGenBuffers(1, &dataBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, dataBufferObject);
	glBufferData(GL_ARRAY_BUFFER, stringLength * 4 * 1 * sizeof(GLfloat), meshCharIndex.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 0, 0);
	*/
	//Index Buffer Object
	glGenBuffers(1, &m_vboIndex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboIndex);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshIndices.size() * sizeof(GLint), meshIndices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	printf("Vertex: %d Normals: %d, texcoords: %d, index: %d m_triangleSize: %d \n", meshVertices.size(), meshNormals.size(), meshTexcoords.size(), meshIndices.size(), 6 * stringLength);

	m_triangleSize = meshIndices.size();

}

void TextMesh::UpdateText(string newText)
{
	SetText(newText);

	int stringLength = m_textString.size();
	vector<GLfloat> meshTexcoords;

	for (int i = 0; i < stringLength; ++i) {
		
		int found = s_charMap.find_first_of(m_textString[i]);
		if (found == string::npos)
		{
			found = 0;
		}

		float leftUV = (float)found / 64.0f;
		float rightUV = ((float)found + 1.0f) / 64.0f;

		meshTexcoords.push_back(leftUV);
		meshTexcoords.push_back(0.0f);

		meshTexcoords.push_back(leftUV);
		meshTexcoords.push_back(1.0f);

		meshTexcoords.push_back(rightUV);
		meshTexcoords.push_back(1.0f);

		meshTexcoords.push_back(rightUV);
		meshTexcoords.push_back(0.0f);
	}

	GLuint texcoordBufferObject;
	GLuint dataBufferObject;
	GLuint normalBufferObject;
	
	glBindBuffer(GL_ARRAY_BUFFER, m_texcoordBufferObject);
	glBufferData(GL_ARRAY_BUFFER, meshTexcoords.size() * sizeof(GLfloat), meshTexcoords.data(), GL_STATIC_DRAW);

	//glEnableVertexAttribArray(2);
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
	//m_triangleSize = meshIndices.size();
}