///////////////////////////////////////////////////////////
//
// Copyright(c) 2017 Rasmus Karlsson
//
///////////////////////////////////////////////////////////

#include "Entity.h"
#include "Cube.h"


Cube::Cube()
{
	Create();
}

void Cube::Create()
{
	static GLfloat vertexBuffer[] = {
		-1.0*0.5, -1.0*0.5, -1.0*0.5, // 0
		1.0*0.5, -1.0*0.5, -1.0*0.5, // 1
		1.0*0.5,  1.0*0.5, -1.0*0.5, // 2
		-1.0*0.5,  1.0*0.5, -1.0*0.5, // 3
		-1.0*0.5, -1.0*0.5,  1.0*0.5, // 4
		1.0*0.5, -1.0*0.5,  1.0*0.5, // 5
		1.0*0.5,  1.0*0.5,  1.0*0.5, // 6
		-1.0*0.5,  1.0*0.5,  1.0*0.5 // 7
	};
	
	m_bbox.SetBoundingBox(vec3(-0.5), vec3(0.5));

	static GLfloat normalBuffer[] = {
		-1.0, -1.0, -1.0, // 0
		1.0, -1.0, -1.0, // 1
		1.0,  1.0, -1.0, // 2
		-1.0,  1.0, -1.0, // 3
		-1.0, -1.0,  1.0, // 4
		1.0, -1.0,  1.0, // 5
		1.0,  1.0,  1.0, // 6
		-1.0,  1.0,  1.0 // 7
	};

	static GLfloat texcoordBuffer[] = {
		0.75f, 1 / 3.0f,
		0.75f, 2 / 3.0f,
		0.5f,  2 / 3.0f,
		0.5f,  1 / 3.0f,
		0.0f,  1 / 3.0f,
		0.0f,  2 / 3.0f,
		0.25f, 2 / 3.0f,
		0.25f, 1 / 3.0f,
	};
	
	static GLfloat colorBuffer[] = {
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f
	};
	
	static GLint indexBuffer[] = {
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
	

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	//Vertex Buffer
	glGenBuffers(1, &m_vertexArrayBuffers.position);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers.position);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), vertexBuffer, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//Vertex Normal Buffer
	glGenBuffers(1, &m_vertexArrayBuffers.normal);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers.normal);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), normalBuffer, GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//Vertex Texcoord Buffer
	glGenBuffers(1, &m_vertexArrayBuffers.texcoord);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers.texcoord);
	glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(GLfloat), texcoordBuffer, GL_STATIC_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

	//Vertex Color Buffer
	glGenBuffers(1, &m_vertexArrayBuffers.color);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers.color);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), colorBuffer, GL_STATIC_DRAW);

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glGenBuffers(1, &m_vboIndex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboIndex);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(GLint), indexBuffer, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	m_triangleSize = 36;
}
