///////////////////////////////////////////////////////////
//
// Copyright(c) 2017 Rasmus Karlsson
//
///////////////////////////////////////////////////////////

#pragma once
#include "TextMesh.h"
#include <algorithm>

#include "MathHelpers.h"
#include "Renderer.h"

TextMesh::TextMesh()
{
	m_stringSize = 0;
	m_stringCapacity = 32;

	SetRenderStyle(RENGINE::UI);

	Create();
}

void TextMesh::SetText(string text)
{
	m_textString = text;
	std::transform(m_textString.begin(), m_textString.end(), m_textString.begin(), ::tolower);
	m_stringSize = m_textString.size();

	if (m_stringSize > m_stringCapacity)
	{
		m_stringCapacity = MathHelpers::GetNextPowerOfTwo(m_textString.size());
		Delete();
		Create();
	}

}

void TextMesh::Create()
{
	vector<int> meshIndices;

	for (uint32 i = 0; i < m_stringCapacity; ++i) {

		meshIndices.push_back(4 * i + 0);
		meshIndices.push_back(4 * i + 1);
		meshIndices.push_back(4 * i + 2);

		meshIndices.push_back(4 * i + 2);
		meshIndices.push_back(4 * i + 3);
		meshIndices.push_back(4 * i + 0);
	}

	
	vector<RENGINE::SVF_PT> vertexBuffer;
	for (uint32 i = 0; i < m_stringCapacity; i++)
	{
		float fI = static_cast<float>(i);
		
		RENGINE::SVF_PT vertex[4];
		vertex[0].position = vec3(2.0f*fI, 1.0f, 0.0f);
		vertex[1].position = vec3(2.0f*fI, -1.0f, 0.0f);
		vertex[2].position = vec3(2.0f*(fI + 1.0f), -1.0f, 0.0f);
		vertex[3].position = vec3(2.0f*(fI + 1.0f), 1.0f, 0.0f);
		
		float leftUV = 0.0f;
		float rightUV = 0.0f;
		
		vertex[0].texcoord = vec2(leftUV, 0.0f);
		vertex[1].texcoord = vec2(leftUV, 1.0f);
		vertex[2].texcoord = vec2(rightUV, 1.0f);
		vertex[3].texcoord = vec2(rightUV, 0.0f);
		
		vertexBuffer.push_back(vertex[0]);
		vertexBuffer.push_back(vertex[1]);
		vertexBuffer.push_back(vertex[2]);
		vertexBuffer.push_back(vertex[3]);
	}

	Renderer::CreateMesh(
		"TextMesh",
		RENGINE::VF_PT,
		RENGINE::VF_PT_SIZE,
		vertexBuffer.size(),
		vertexBuffer.data(),
		m_combinedVBO,
		m_vao,
		m_vboIndex,
		meshIndices.size(),
		meshIndices.data()
	);
	
	m_triangleCount = vertexBuffer.size();
	m_indexSize = meshIndices.size();

	m_created = true;
}

void TextMesh::UpdateTextBuffer(string newText)
{
	//SetText(std::move(newText));
	SetText(newText);
	
	const uint32 stringLength = m_textString.size();
	vector<float> meshTexcoords;

	vector<RENGINE::SVF_PT> vertexBuffer;
	for (uint32 i = 0; i < m_stringCapacity; i++)
	{
		float fI = static_cast<float>(i);

		RENGINE::SVF_PT vertex[4];
		vertex[0].position = vec3(2.0f*fI, 1.0f, 0.0f);
		vertex[1].position = vec3(2.0f*fI, -1.0f, 0.0f);
		vertex[2].position = vec3(2.0f*(fI + 1.0f), -1.0f, 0.0f);
		vertex[3].position = vec3(2.0f*(fI + 1.0f), 1.0f, 0.0f);

		uint32 found = 0;
		if (i < stringLength) found = s_charMap.find_first_of(m_textString[i]);
		if (found == string::npos)
		{
			found = 25;
		}

		float leftUV = static_cast<float>(found) / 64.0f;
		float rightUV = (static_cast<float>(found) + 1.0f) / 64.0f;

		vertex[0].texcoord = vec2(leftUV, 0.0f);
		vertex[1].texcoord = vec2(leftUV, 1.0f);
		vertex[2].texcoord = vec2(rightUV, 1.0f);
		vertex[3].texcoord = vec2(rightUV, 0.0f);

		vertexBuffer.push_back(vertex[0]);
		vertexBuffer.push_back(vertex[1]);
		vertexBuffer.push_back(vertex[2]);
		vertexBuffer.push_back(vertex[3]);
	}

	Renderer::UpdateSubVertexBuffer(m_combinedVBO, 0, RENGINE::VF_PT_SIZE, vertexBuffer.size(), vertexBuffer.data());

	m_indexSize = m_textString.size() * 6;
}