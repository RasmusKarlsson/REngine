#pragma once
//#include <lodepng.cpp>
#include "Entity.h"
#include "TerrainMesh.h"
#include "MathHelpers.h"

using namespace std;

int C_TERRAINLODS = 8;

float ReadHeightValue(vector<unsigned char> &imageData, int textureSize, int x, int y, int offsetX, int offsetY);

TerrainMesh::TerrainMesh()
{
	//Create(64,64);
	m_lodBufferObjects.resize(C_TERRAINLODS);
	m_lodTriangleSize.resize(C_TERRAINLODS);
	m_currentLod = 0;
}

TerrainMesh::~TerrainMesh()
{
	Delete();
}

void TerrainMesh::Create(int width, int depth)
{
	m_resolution = ivec2(8, 8);
	m_heightScale = 1.0f;
	m_size = 10.0f;

	GLfloat vertexBuffer[64*3] = {0.0f};

	vector<GLfloat> vertices;
	vector<GLfloat> normals;
	vector<GLfloat> texcoords;
	vector<GLuint> indices;

	vertices.resize(width * depth * 3);
	normals.resize(width * depth * 3);
	texcoords.resize(width * depth * 2);
	indices.resize((width-1) * (depth-1) * 6);

	int v = 0;
	int tc = 0;
	for (int x = 0; x < width; x++)
	{
		for (int z = 0; z < depth; z++)
		{
			vertices[v] = (float)x;
			normals[v++] = 0.0f;

			vertices[v] = 0.0f;
			normals[v++] = 1.0f;

			vertices[v] = (float)z;
			normals[v++] = 0.0f;

			texcoords[tc++] = (float)x;
			texcoords[tc++] = (float)z;
		}
	}

	int d = 0;
	for (int j = 0; j < (depth - 1); j++)
	{
		for (int i = 0; i < (width - 1); i++)
		{
			int idx = j*(width)+i;
			indices[d++] = idx;
			indices[d++] = (idx + width);
			indices[d++] = (idx + width + 1);

			indices[d++] = idx;
			indices[d++] = (idx + width + 1);
			indices[d++] = (idx + 1);
		}
	}

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	glGenBuffers(1, &m_vboVertex);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboVertex);
	glBufferData(GL_ARRAY_BUFFER, width * depth * 3 * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	GLuint normalBufferObject;
	glGenBuffers(1, &normalBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferObject);
	glBufferData(GL_ARRAY_BUFFER, width * depth * 3 * sizeof(GLfloat), normals.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	GLuint texcoordBufferObject;
	glGenBuffers(1, &texcoordBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, texcoordBufferObject);
	glBufferData(GL_ARRAY_BUFFER, width * depth * 2 * sizeof(GLfloat), texcoords.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glGenBuffers(1, &m_vboIndex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboIndex);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (width - 1) * (depth - 1) * 6 * sizeof(GLint), indices.data() , GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	m_triangleSize = (width - 1) * (depth - 1) * 6;
}


void TerrainMesh::CreateFromHeightmap(Texture* texture)
{
	unsigned int width = texture->GetWidth();
	unsigned int depth = texture->GetHeight();

	//Heightmap too small for C_TERRAINLODS amount of lods?
	if ((int)log2((float)(width - 1)) < C_TERRAINLODS)
	{
		C_TERRAINLODS = (int)log2((float)(width - 1));
		m_lodBufferObjects.resize(C_TERRAINLODS);
		m_lodTriangleSize.resize(C_TERRAINLODS);
	}

	
	vector<unsigned char> imageData = texture->GetImageData();

	m_resolution = ivec2(width, depth);
	m_heightScale = 1.0f;
	m_size = 0.1f;


	m_bbox->SetBoundingBox(vec3(0.0f), vec3(width, m_size*m_heightScale*256.0f, depth));

	GLfloat vertexBuffer[64 * 3] = { 0.0f };

	vector<GLfloat> vertices;
	vector<GLfloat> normals;
	vector<GLfloat> texcoords;
	vector<GLuint> indices;

	vertices.resize(width * depth * 3);
	normals.resize(width * depth * 3);
	texcoords.resize(width * depth * 2);
	indices.resize((width - 1) * (depth - 1) * 6);

	int v = 0;
	int tc = 0;
	for (int x = 0; x < width; x++)
	{
		for (int z = 0; z < depth; z++)
		{
			float height = m_size*ReadHeightValue(imageData, width, x, z, 0, 0);
			vec3 normal = vec3(0.0f,0.0f,1.0f);
			if (x + 1 < width && z + 1 < depth)
			{
				float x1 = m_size*ReadHeightValue(imageData, width, x+1, z, 0, 0);
				float z1 = m_size*ReadHeightValue(imageData, width, x, z+1, 0, 0);
				float z1x1 = m_size*ReadHeightValue(imageData, width, x+1, z+1, 0, 0);
				vec3 vx = normalize(cross(normalize(vec3(2.0f, 0.0f,height - x1)), normalize(vec3(0.0f, 2.0f, height - z1))));
				//vec3 vz = normalize(cross(vec3(0.0f, 0.0f, 1.0f), );
				normal = normalize(vx);
			}

			vertices[v] = (float)x;
			normals[v++] = normal.x;

			vertices[v] = m_heightScale * height;
			normals[v++] = normal.z;

			vertices[v] = (float)z;
			normals[v++] = normal.y;

			texcoords[tc++] = (float)x;
			texcoords[tc++] = (float)z;
		}
	}

	int d = 0;
	
	for (int j = 0; j < (depth - 1); j++)
	{
		for (int i = 0; i < (width - 1); i++)
		{
			int idx = j*(width)+i;
			indices[d++] = idx;
			indices[d++] = (idx + width);
			indices[d++] = (idx + width + 1);

			indices[d++] = idx;
			indices[d++] = (idx + width + 1);
			indices[d++] = (idx + 1);
		}
	}

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	glGenBuffers(1, &m_vboVertex);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboVertex);
	glBufferData(GL_ARRAY_BUFFER, width * depth * 3 * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	GLuint normalBufferObject;
	glGenBuffers(1, &normalBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferObject);
	glBufferData(GL_ARRAY_BUFFER, width * depth * 3 * sizeof(GLfloat), normals.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	GLuint texcoordBufferObject;
	glGenBuffers(1, &texcoordBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, texcoordBufferObject);
	glBufferData(GL_ARRAY_BUFFER, width * depth * 2 * sizeof(GLfloat), texcoords.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

	vector<GLuint> lodIndices;
	bool transpose = false;
	for (int lodIndex = C_TERRAINLODS-1; lodIndex >= 0; lodIndex--)
	{
		d = 0;
		int lod = MathHelpers::CounterToPowerOfTwo(lodIndex);
		lodIndices.resize((width - 1) /lod * (depth - 1)/lod * 6);
		for (int j = 0; j < (depth - 1); j += lod)
		{
			for (int i = 0; i < (width - 1); i += lod)
			{
				int idx = j*(width)+i;
				lodIndices[d++] = idx;
				lodIndices[d++] = (idx + lod * width);
				//Criss cross trangle binding
				if (!transpose)
				{
					lodIndices[d++] = (idx + lod * width + lod);
					lodIndices[d++] = idx;
				}
				else
				{
					lodIndices[d++] = (idx + lod);
					lodIndices[d++] = (idx + lod * width);
				}		
				lodIndices[d++] = (idx + lod * width + lod);
				lodIndices[d++] = (idx + lod);
				transpose = !transpose;
			}
			transpose = !transpose;
		}

		m_lodTriangleSize[lodIndex] = (width - 1) / lod * (depth - 1) / lod * 6;

		glGenBuffers(1, &m_lodBufferObjects[lodIndex]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_lodBufferObjects[lodIndex]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (width - 1) / lod * (depth - 1) / lod * 6 * sizeof(GLint), lodIndices.data(), GL_STATIC_DRAW);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	m_triangleSize = (width - 1) * (depth - 1) * 6;
}

float ReadHeightValue(vector<unsigned char> &imageData, int textureSize ,int x, int y, int offsetX, int offsetY)
{
	int xPos = offsetX + x;
	int yPos = offsetY + y;
	int index = 4 * (yPos*textureSize + xPos);
	return static_cast<float>(imageData[index]);
}

void TerrainMesh::CreatePatchFromHeightmap(Texture* texture, int xStart, int yStart, int patchSize)
{
	unsigned int width = patchSize;
	unsigned int depth = patchSize;

	//Heightmap too small for C_TERRAINLODS amount of lods?
	if ((int)log2((float)(patchSize - 1)) < C_TERRAINLODS)
	{
		C_TERRAINLODS = (int)log2((float)(patchSize - 1));
		m_lodBufferObjects.resize(C_TERRAINLODS);
		m_lodTriangleSize.resize(C_TERRAINLODS);
		SetNrLods(C_TERRAINLODS);
	}

	vector<unsigned char> imageData = texture->GetImageData();

	int realSize = texture->GetWidth();

	m_resolution = ivec2(width, depth);
	m_heightScale = 1.0f;
	m_size = 0.1f;

	m_bbox->SetBoundingBox(vec3(0.0f), vec3(width, m_size*m_heightScale*256.0f, depth));

	vector<GLfloat> vertices;
	vector<GLfloat> normals;
	vector<GLfloat> texcoords;
	vector<GLuint> indices;

	vertices.resize(width * depth * 3);
	normals.resize(width * depth * 3);
	texcoords.resize(width * depth * 2);
	indices.resize((width - 1) * (depth - 1) * 6);

	int v = 0;
	int tc = 0;
	for (int x = 0; x < width; x++)
	{
		for (int z = 0; z < depth; z++)
		{
			float height = m_size*ReadHeightValue(imageData, realSize, x, z, xStart, yStart);
			vec3 normal = vec3(0.0f, 0.0f, 1.0f);
			if (xStart+x + 1 < realSize && yStart+z + 1 < realSize)
			{
				float x1 = m_size*ReadHeightValue(imageData, realSize, x+1, z, xStart, yStart);
				float z1 = m_size*ReadHeightValue(imageData, realSize, x, z+1, xStart, yStart);
				float z1x1 = m_size*ReadHeightValue(imageData, realSize, x+1, z + 1, xStart, yStart);
				vec3 vx = normalize(cross(normalize(vec3(2.0f, 0.0f, height - x1)), normalize(vec3(0.0f, 2.0f, height - z1))));
				//vec3 vz = normalize(cross(vec3(0.0f, 0.0f, 1.0f), );
				normal = normalize(vx);
			}

			vertices[v] = (float)x - patchSize / 2.0f; //Center the patch position
			normals[v++] = normal.x;

			vertices[v] = m_heightScale * height;
			normals[v++] = normal.z;

			vertices[v] = (float)z - patchSize / 2.0f;
			normals[v++] = normal.y;

			texcoords[tc++] = (float)x + (float)xStart;
			texcoords[tc++] = (float)z + (float)yStart;
		}
	}

	int d = 0;
	/*
	for (int j = 0; j < (depth - 1); j++)
	{
		for (int i = 0; i < (width - 1); i++)
		{
			int idx = j*(width)+i;
			indices[d++] = idx;
			indices[d++] = (idx + width);
			indices[d++] = (idx + width + 1);

			indices[d++] = idx;
			indices[d++] = (idx + width + 1);
			indices[d++] = (idx + 1);
		}
	}*/

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	glGenBuffers(1, &m_vboVertex);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboVertex);
	glBufferData(GL_ARRAY_BUFFER, width * depth * 3 * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	GLuint normalBufferObject;
	glGenBuffers(1, &normalBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferObject);
	glBufferData(GL_ARRAY_BUFFER, width * depth * 3 * sizeof(GLfloat), normals.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	GLuint texcoordBufferObject;
	glGenBuffers(1, &texcoordBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, texcoordBufferObject);
	glBufferData(GL_ARRAY_BUFFER, width * depth * 2 * sizeof(GLfloat), texcoords.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

	vector<GLuint> lodIndices;
	bool transpose = false;
	for (int lodIndex = C_TERRAINLODS - 1; lodIndex >= 0; lodIndex--)
	{
		d = 0;
		int lod = MathHelpers::CounterToPowerOfTwo(lodIndex);
		lodIndices.resize((width - 1) / lod * (depth - 1) / lod * 6);
		for (int j = 0; j < (depth - 1); j += lod)
		{
			for (int i = 0; i < (width - 1); i += lod)
			{
				int idx = (j)*(width)+(i);
				lodIndices[d++] = idx;
				lodIndices[d++] = (idx + lod * width);
				//Criss cross trangle binding
				if (!transpose)
				{
					lodIndices[d++] = (idx + lod * width + lod);
					lodIndices[d++] = idx;
				}
				else
				{
					lodIndices[d++] = (idx + lod);
					lodIndices[d++] = (idx + lod * width);
				}
				lodIndices[d++] = (idx + lod * width + lod);
				lodIndices[d++] = (idx + lod);
				transpose = !transpose;
			}
			transpose = !transpose;
		}

		m_lodTriangleSize[lodIndex] = (width - 1) / lod * (depth - 1) / lod * 6;

		glGenBuffers(1, &m_lodBufferObjects[lodIndex]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_lodBufferObjects[lodIndex]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (width - 1) / lod * (depth - 1) / lod * 6 * sizeof(GLint), lodIndices.data(), GL_STATIC_DRAW);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	m_triangleSize = (width - 1) * (depth - 1) * 6;
}

void TerrainMesh::SetLod(int lod)
{
	if (m_currentLod == lod) return;
	m_currentLod = lod;
	m_triangleSize = m_lodTriangleSize[lod];
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_lodBufferObjects[m_currentLod]);
}

