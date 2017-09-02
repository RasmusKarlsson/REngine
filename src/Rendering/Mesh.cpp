///////////////////////////////////////////////////////////
//
// Copyright(c) 2017 Rasmus Karlsson
//
///////////////////////////////////////////////////////////

#include "Entity.h"
#include "Mesh.h"
#include "loadobj.h"

Mesh::Mesh()
{
}


Mesh::~Mesh()
{
}

void Mesh::LoadMeshFromObjFile(string fileName)
{
	Model *m;

	char *cstrName = &fileName[0u];

	m = LoadModel(cstrName);


	GLuint normalBufferObject;
	GLuint texcoordBufferObject;
	GLuint colorBufferObject;

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vboVertex);
	glGenBuffers(1, &m_vboIndex);
	glGenBuffers(1, &normalBufferObject);

	if (m->texCoordArray != NULL)
		glGenBuffers(1, &m->tb);

	if (m->tangentArray != NULL)
	{
		glGenBuffers(1, &m->tab);
	}

	if (m->bitangentArray != NULL)
	{
		glGenBuffers(1, &m->btb);
	}


	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, m_vboVertex);
	glBufferData(GL_ARRAY_BUFFER, m->numVertices * 3 * sizeof(GLfloat), m->vertexArray, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);

	// VBO for normal data
	glBindBuffer(GL_ARRAY_BUFFER, m->nb);
	glBufferData(GL_ARRAY_BUFFER, m->numVertices * 3 * sizeof(GLfloat), m->normalArray, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);

	// VBO for texture coordinate data NEW for 5b
	if (m->texCoordArray != NULL)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m->tb);
		glBufferData(GL_ARRAY_BUFFER, m->numVertices * 2 * sizeof(GLfloat), m->texCoordArray, GL_STATIC_DRAW);
		glEnableVertexAttribArray(2);
	}
	//tangent
	/*
	if (m->tangentArray != NULL && tangentVariableName)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m->tab);
		glBufferData(GL_ARRAY_BUFFER, m->numVertices * 3 * sizeof(GLfloat), m->tangentArray, GL_STATIC_DRAW);
		glVertexAttribPointer(glGetAttribLocation(program, tangentVariableName), 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(glGetAttribLocation(program, tangentVariableName));
	}

	if (m->bitangentArray != NULL && bitangentVariableName)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m->btb);
		glBufferData(GL_ARRAY_BUFFER, m->numVertices * 3 * sizeof(GLfloat), m->bitangentArray, GL_STATIC_DRAW);
		glVertexAttribPointer(glGetAttribLocation(program, bitangentVariableName), 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(glGetAttribLocation(program, bitangentVariableName));
	}*/

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboIndex);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m->numIndices*sizeof(GLuint), m->indexArray, GL_STATIC_DRAW);

	m_triangleSize = m->numIndices;
}