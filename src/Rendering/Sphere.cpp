///////////////////////////////////////////////////////////
//
// Copyright(c) 2017 Rasmus Karlsson
//
///////////////////////////////////////////////////////////

#include "Sphere.h"

Sphere::Sphere()
{
	m_isInited = false;
	m_vao = 0;
	m_vboIndex = 0;

	m_lats = 40;
	m_longs = 40;
	Create();
}

void Sphere::Create()
{
	std::vector<GLfloat> sphere_vertices;
	std::vector<GLfloat> sphere_normals;
	std::vector<GLfloat> sphere_texcoords;
	std::vector<GLushort> sphere_indices;
	int sectors = 40;
	float radius = 1.0f;
	float M_PI = 3.141592f;
	float M_PI_2 = 2.0f*M_PI;
	float const R = 1.0f / static_cast<float>(m_lats - 1);
	float const S = 1.0f / static_cast<float>(m_longs - 1);
	int r, s;

	sphere_vertices.resize(m_lats * sectors * 3);
	sphere_normals.resize(m_lats * sectors * 3);
	sphere_texcoords.resize(m_lats * sectors * 2);
	std::vector<GLfloat>::iterator v = sphere_vertices.begin();
	std::vector<GLfloat>::iterator n = sphere_normals.begin();
	std::vector<GLfloat>::iterator t = sphere_texcoords.begin();
	for (r = 0; r < sectors; r++) for (s = 0; s < sectors; s++) {
		float const y = sin(-M_PI_2 + M_PI * r * R);
		float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
		float const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

		*t++ = s*S;
		*t++ = r*R;

		*v++ = x * radius;
		*v++ = y * radius;
		*v++ = z * radius;

		*n++ = x;
		*n++ = y;
		*n++ = z;
	}

	sphere_indices.resize(m_lats * sectors * 4);
	std::vector<GLushort>::iterator i = sphere_indices.begin();
	for (r = 0; r < m_lats; r++) for (s = 0; s < sectors; s++) {
		*i++ = r * sectors + s;
		*i++ = r * sectors + (s + 1);
		*i++ = (r + 1) * sectors + (s + 1);
		*i++ = (r + 1) * sectors + s;
	}

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vertexArrayBuffers.position);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers.position);
	glBufferData(GL_ARRAY_BUFFER, sphere_vertices.size() * sizeof(GLfloat), sphere_vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &m_vboIndex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboIndex);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphere_indices.size() * sizeof(GLint), sphere_indices.data(), GL_STATIC_DRAW);

	m_triangleCount = sphere_indices.size();

	m_isInited = true;
}

/*
void Sphere::draw()
{
	if (!isInited) {
		std::cout << "please call init() before draw()" << std::endl;
	}

	// draw sphere
	glBindVertexArray(m_vao);
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(GL_PRIMITIVE_RESTART_FIXED_INDEX);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboIndex);
	glDrawElements(GL_QUAD_STRIP, numsToDraw, GL_UNSIGNED_INT, NULL);
}
*/