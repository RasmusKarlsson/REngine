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
	SetRenderStyle(RENDERSTYLE_2D);
}

TextMesh::~TextMesh()
{

}

void TextMesh::SetText(string text)
{
	m_textString = text;

	std::transform(m_textString.begin(), m_textString.end(), m_textString.begin(), ::tolower);

	m_stringSize = m_textString.size();

	m_indexSize = text.size() * 6;
}

void TextMesh::Create()
{
	//const uint32 stringLength = static_cast<uint32>(m_textString.size());
	const uint32 stringLength = 256;

	m_stringCapacity = stringLength;
	
	vector<float> meshVertices;
	vector<float> meshTexcoords;
	vector<int> meshIndices;

	for (uint32 i = 0; i < stringLength; ++i) {
		float fI = static_cast<float>(i);
		meshVertices.push_back(2.0f*fI);
		meshVertices.push_back(1.0f);
		//meshVertices.push_back(0.0f);

		meshVertices.push_back(2.0f*fI);
		meshVertices.push_back(-1.0f);
		//meshVertices.push_back(0.0f);

		meshVertices.push_back(2.0f*(fI + 1.0f));
		meshVertices.push_back(-1.0f);
		//meshVertices.push_back(0.0f);

		meshVertices.push_back(2.0f*(fI + 1.0f));
		meshVertices.push_back(1.0f);
		//meshVertices.push_back(0.0f);

		uint32 found = 0;// s_charMap.find_first_of(m_textString[i]);
		//if (found == string::npos)
		{
			found = 0;
		}

		float leftUV = static_cast<float>(found) / 64.0f;
		float rightUV = (static_cast<float>(found) + 1.0f) / 64.0f;

		meshTexcoords.push_back(leftUV);
		meshTexcoords.push_back(0.0f);

		meshTexcoords.push_back(leftUV);
		meshTexcoords.push_back(1.0f);

		meshTexcoords.push_back(rightUV);
		meshTexcoords.push_back(1.0f);

		meshTexcoords.push_back(rightUV);
		meshTexcoords.push_back(0.0f);

		meshIndices.push_back(4 * i + 0);
		meshIndices.push_back(4 * i + 1);
		meshIndices.push_back(4 * i + 2);

		meshIndices.push_back(4 * i + 2);
		meshIndices.push_back(4 * i + 3);
		meshIndices.push_back(4 * i + 0);
	}

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	//Vertex Buffer
	glGenBuffers(1, &m_vertexArrayBuffers.position);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers.position);
	glBufferData(GL_ARRAY_BUFFER, meshVertices.size() * sizeof(GLfloat), meshVertices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	//Vertex Texcoord Buffer
	glGenBuffers(1, &m_vertexArrayBuffers.texcoord);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers.texcoord);
	glBufferData(GL_ARRAY_BUFFER, meshTexcoords.size() * sizeof(GLfloat), meshTexcoords.data(), GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	//Index Buffer Object
	glGenBuffers(1, &m_vboIndex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboIndex);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshIndices.size() * sizeof(GLint), meshIndices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
	m_triangleCount = meshVertices.size();
	m_indexSize = meshIndices.size();

	m_created = true;
}

void TextMesh::UpdateText(string newText)
{
	const uint32 oldTextStringSize = static_cast<uint32>(m_textString.size());

	SetText(newText);
	/*
	if (m_textString.capacity() < m_stringSize)
	{
		Create();
		printf("Redid text! Old size: %d new size: %d\n", oldTextStringSize, m_stringSize);
		return;
	}
	*/
	const int stringLength = m_textString.size();
	vector<float> meshTexcoords;

	for (int i = 0; i < stringLength; ++i) {
		
		int found = s_charMap.find_first_of(m_textString[i]);
		if (found == string::npos)
		{
			found = 0;
		}

		float leftUV = static_cast<float>(found) / 64.0f;
		float rightUV = (static_cast<float>(found) + 1.0f) / 64.0f;

		meshTexcoords.push_back(leftUV);
		meshTexcoords.push_back(0.0f);

		meshTexcoords.push_back(leftUV);
		meshTexcoords.push_back(1.0f);

		meshTexcoords.push_back(rightUV);
		meshTexcoords.push_back(1.0f);

		meshTexcoords.push_back(rightUV);
		meshTexcoords.push_back(0.0f);
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers.texcoord);
	glBufferSubData(GL_ARRAY_BUFFER, 0, meshTexcoords.size() * sizeof(GLfloat), meshTexcoords.data());
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}