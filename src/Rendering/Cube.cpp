///////////////////////////////////////////////////////////
//
// Copyright(c) 2017 Rasmus Karlsson
//
///////////////////////////////////////////////////////////

#include "Entity.h"
#include "Cube.h"

#include "Renderer.h"


Cube::Cube()
{
	Create();
}

void Cube::Create()
{
	const float size = 1.0f;
	const vector<vec3> positionBuffer = {
		vec3(-0.5f*size, -0.5f*size, -0.5f*size), // 0
		vec3(0.5f*size, -0.5f*size, -0.5f*size), // 1
		vec3(0.5f*size,  0.5f*size, -0.5f*size), // 2
		vec3(-0.5f*size,  0.5f*size, -0.5f*size), // 3
		vec3(-0.5f*size, -0.5f*size,  0.5f*size), // 4
		vec3(0.5f*size, -0.5f*size,  0.5f*size), // 5
		vec3(0.5f*size,  0.5f*size,  0.5f*size), // 6
		vec3(-0.5f*size,  0.5f*size,  0.5f*size)  // 7
	};
	
	m_bbox.SetBoundingBox(vec3(-0.5*size), vec3(0.5*size));

	const vector<vec3> normalBuffer = {
		vec3(-1.0, -1.0, -1.0), // 0
		vec3(1.0, -1.0, -1.0), // 1
		vec3(1.0,  1.0, -1.0), // 2
		vec3(-1.0,  1.0, -1.0), // 3
		vec3(-1.0, -1.0,  1.0), // 4
		vec3(1.0, -1.0,  1.0), // 5
		vec3(1.0,  1.0,  1.0), // 6
		vec3(-1.0,  1.0,  1.0)  // 7
	};

	//TODO: Fix better texcoords
	const vector<vec2> texcoordBuffer = {
		vec2(0.75f, 1 / 3.0f),
		vec2(0.75f, 2 / 3.0f),
		vec2(0.5f,  2 / 3.0f),
		vec2(0.5f,  1 / 3.0f),
		vec2(0.0f,  1 / 3.0f),
		vec2(0.0f,  2 / 3.0f),
		vec2(0.25f, 2 / 3.0f),
		vec2(0.25f, 1 / 3.0f),
	};
	
	const vector<vec3> colorBuffer = {
		vec3(1.0f, 1.0f, 1.0f),
		vec3(1.0f, 1.0f, 1.0f),
		vec3(1.0f, 1.0f, 1.0f),
		vec3(1.0f, 1.0f, 1.0f),
		vec3(1.0f, 1.0f, 1.0f),
		vec3(1.0f, 1.0f, 1.0f),
		vec3(1.0f, 1.0f, 1.0f),
		vec3(1.0f, 1.0f, 1.0f)
	};
	
	vector<int> indexBuffer = {
		0,2,1, //Front
		0,3,2,

		1,2,6, //Back
		6,5,1,

		4,5,6,
		6,7,4,

		2,3,6,
		6,3,7,

		0,7,3,
		0,4,7,

		0,1,5,
		0,5,4
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
		"Cube", 
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
