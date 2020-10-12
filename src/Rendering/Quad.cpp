///////////////////////////////////////////////////////////
//
// Copyright(c) 2017 Rasmus Karlsson
//
///////////////////////////////////////////////////////////

#include "Entity.h"
#include "Quad.h"

#include "Renderer.h"

Quad::Quad()
{
	Create();
}

void Quad::Create()
{
	static vector<vec3> positionBuffer = {
		vec3(-1.0f, -1.0f, 0.0f),
		vec3(-1.0f,  1.0f, 0.0f),
		vec3(1.0f,  1.0f, 0.0f),
		vec3(1.0f, -1.0f, 0.0f)
	};

	m_bbox.SetBoundingBox(vec3(-1.0f, -1.0f, -0.001f), vec3(1.0f, 1.0f, 0.001f));

	static vector<vec3> normalBuffer = {
		vec3(0.0f, 0.0f, 1.0f),
		vec3(0.0f, 0.0f, 1.0f),
		vec3(0.0f, 0.0f, 1.0f),
		vec3(0.0f, 0.0f, 1.0f)
	};	

	static vector<vec2> texcoordBuffer = {
		vec2(0.0f, 1.0f),
		vec2(0.0f, 0.0f),
		vec2(1.0f, 0.0f),
		vec2(1.0f, 1.0f)
	};

	static vector<vec3> colorBuffer = {
		vec3(1.0f, 1.0f, 1.0f),
		vec3(1.0f, 1.0f, 1.0f),
		vec3(1.0f, 1.0f, 1.0f),
		vec3(1.0f, 1.0f, 1.0f)
	};

	static vector<int> indexBuffer = {
		0,2,1,
		0,3,2
	};


	vector<RENGINE::SVF_PNTC> vertexBuffer;
	for (uint32 i = 0; i < positionBuffer.size(); i++)
	{
		RENGINE::SVF_PNTC vertex;

		vertex.position = positionBuffer[i];
		vertex.normal = normalBuffer[i];
		vertex.texcoord = texcoordBuffer[i];
		vertex.color = colorBuffer[i];
		vertexBuffer.push_back(vertex);
	}

	Renderer::CreateMesh(
		"Quad",
		RENGINE::VF_PNTC,
		RENGINE::VF_PNTC_SIZE,
		vertexBuffer.size(),
		vertexBuffer.data(),
		m_combinedVBO,
		m_vao,
		m_vboIndex,
		indexBuffer.size(),
		indexBuffer.data()
	);

	m_triangleCount = vertexBuffer.size();
	m_indexSize = indexBuffer.size();
	
	m_created = true;
}
