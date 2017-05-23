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

Cube::~Cube()
{
	Delete();
}

void Cube::Create()
{
	/*
	GLfloat vertexBuffer[] = {
		-1.0*0.5, -1.0*0.5, -1.0*0.5, // 0
		1.0*0.5, -1.0*0.5, -1.0*0.5, // 1
		1.0*0.5,  1.0*0.5, -1.0*0.5, // 2
		-1.0*0.5,  1.0*0.5, -1.0*0.5, // 3
		-1.0*0.5, -1.0*0.5,  1.0*0.5, // 4
		1.0*0.5, -1.0*0.5,  1.0*0.5, // 5
		1.0*0.5,  1.0*0.5,  1.0*0.5, // 6
		-1.0*0.5,  1.0*0.5,  1.0*0.5 // 7
	};*/
	GLfloat vertexBuffer[] = {
	-0.500000, - 0.500000, 0.500000,
	0.500000, - 0.500000, 0.500000,
	- 0.500000, 0.500000, 0.500000,
	0.500000, 0.500000, 0.500000,
	- 0.500000, 0.500000, - 0.500000,
0.500000, 0.500000, - 0.500000,
	- 0.500000, - 0.500000, - 0.500000,
	0.500000, - 0.500000, - 0.500000

	};

	GLfloat normalBuffer[] = {
		-1.0, -1.0, -1.0, // 0
		1.0, -1.0, -1.0, // 1
		1.0,  1.0, -1.0, // 2
		-1.0,  1.0, -1.0, // 3
		-1.0, -1.0,  1.0, // 4
		1.0, -1.0,  1.0, // 5
		1.0,  1.0,  1.0, // 6
		-1.0,  1.0,  1.0 // 7
	};
	/*
	GLfloat texcoordBuffer[] = {
		0.75f, 1 / 3.0f,
		0.75f, 2 / 3.0f,
		0.5f,  2 / 3.0f,
		0.5f,  1 / 3.0f,
		0.0f,  1 / 3.0f,
		0.0f,  2 / 3.0f,
		0.25f, 2 / 3.0f,
		0.25f, 1 / 3.0f,
	};*/
	GLfloat texcoordBuffer[] = {
	0.250000, 1.000000,
	0.000000, 0.333000,
	0.250000, 0.667000,
	0.250000, 0.333000,
	0.500000, 0.667000,
	0.500000, 0.333000,
	0.750000, 0.667000,
	0.750000, 0.333000,
	1.000000, 0.667000,
	1.000000, 0.333000,
	0.500000, 0.000000,
	0.500000, 0.333000,
	0.500000, 1.000000,
	0.500000, 0.667000,
	0.250000, 0.000000,
	0.250000, 0.333000,
	0.000000, 0.667000,
	0.250000, 0.667000

	};
	GLfloat colorBuffer[] = {
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f
	};
	/*
	GLint indexBuffer[] = {
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
	*/
	GLint indexBuffer[] = {
		0, 1, 2,
		2, 1, 3,
		2, 3, 4,
		4, 3, 5,
		4, 5, 6,
		6, 5, 7,
		6, 7, 0,
		0, 7, 1,
		1, 7, 3,
		3, 7, 5,
		6, 0, 4,
		4, 0, 2,
	};

	GLuint normalBufferObject;
	GLuint texcoordBufferObject;
	GLuint colorBufferObject;

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	//Vertex Buffer
	glGenBuffers(1, &m_vboVertex);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboVertex);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), vertexBuffer, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//Vertex Normal Buffer
	glGenBuffers(1, &normalBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferObject);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), normalBuffer, GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//Vertex Texcoord Buffer
	glGenBuffers(1, &texcoordBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, texcoordBufferObject);
	glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(GLfloat), texcoordBuffer, GL_STATIC_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

	//Vertex Color Buffer
	glGenBuffers(1, &colorBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferObject);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), colorBuffer, GL_STATIC_DRAW);

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glGenBuffers(1, &m_vboIndex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboIndex);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(GLint), indexBuffer, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	m_triangleSize = 36;
}
