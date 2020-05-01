///////////////////////////////////////////////////////////
//
// Copyright(c) 2017 Rasmus Karlsson
//
///////////////////////////////////////////////////////////

#include "Entity.h"
#include "Quad.h"

Quad::Quad()
{
	Create();
}

void Quad::Create()
{
	static GLfloat vertexBuffer[] = {
		-1.0f, -1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		1.0f,  1.0f, 0.0f,
		1.0f, -1.0f, 0.0f
	};

	m_bbox.SetBoundingBox(vec3(-1.0f, -1.0f, -0.001f), vec3(1.0f, 1.0f, 0.001f));

	static GLfloat normalBuffer[] = {
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f
	};	

	static GLfloat texcoordBuffer[] = {
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f
	};

	static GLfloat colorBuffer[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f
	};

	static GLint indexBuffer[] = {
		0,2,1,
		0,3,2
	};

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	//Vertex Buffer
	glGenBuffers(1, &m_vertexArrayBuffers.position);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers.position);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), vertexBuffer, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//Vertex Normal Buffer
	glGenBuffers(1, &m_vertexArrayBuffers.normal);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers.normal);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), normalBuffer, GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//Vertex Texcoord Buffer
	glGenBuffers(1, &m_vertexArrayBuffers.texcoord);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers.texcoord);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), texcoordBuffer, GL_STATIC_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

	//Vertex Color Buffer
	glGenBuffers(1, &m_vertexArrayBuffers.color);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers.color);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), colorBuffer, GL_STATIC_DRAW);

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//Index Buffer Object
	glGenBuffers(1, &m_vboIndex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboIndex);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLint), indexBuffer, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	m_triangleSize = 6;
}
