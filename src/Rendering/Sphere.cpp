///////////////////////////////////////////////////////////
//
// Copyright(c) 2017 Rasmus Karlsson
//
///////////////////////////////////////////////////////////

#include "Sphere.h"

#include "Renderer.h"

Sphere::Sphere()
{
	Create();
}

void Sphere::Create()
{
	vector<RENGINE::SVF_PNTC> vertexBuffer;
	vector<int> indexBuffer;
	float M_PI = 3.141592f;
	
	double latitudeBands = 12;
	double longitudeBands = 12;
	double radius = 1;
	
	m_bbox.SetBoundingBox(vec3(-radius), vec3(radius));


	for (double latNumber = 0; latNumber <= latitudeBands; latNumber++) {
		double theta = latNumber * M_PI / latitudeBands;
		double sinTheta = sin(theta);
		double cosTheta = cos(theta);

		for (double longNumber = 0; longNumber <= longitudeBands; longNumber++) {
			double phi = longNumber * 2 * M_PI / longitudeBands;
			double sinPhi = sin(phi);
			double cosPhi = cos(phi);

			RENGINE::SVF_PNTC vertex;
			vertex.normal.x = cosPhi * sinTheta;   // x
			vertex.normal.y = cosTheta;            // y
			vertex.normal.z = sinPhi * sinTheta;   // z
			vertex.texcoord.x = 1 - (longNumber / longitudeBands); // u
			vertex.texcoord.y = 1 - (latNumber / latitudeBands);   // v
			vertex.position.x = radius * vertex.normal.x;
			vertex.position.y = radius * vertex.normal.y;
			vertex.position.z = radius * vertex.normal.z;
			vertex.color = vec3(1.0);

			vertexBuffer.push_back(vertex);
		}
	}

	for (int latNumber = 0; latNumber < latitudeBands; latNumber++) {
		for (int longNumber = 0; longNumber < longitudeBands; longNumber++) {
			int first = (latNumber * (longitudeBands + 1)) + longNumber;
			int second = first + longitudeBands + 1;

			indexBuffer.push_back(first);
			indexBuffer.push_back(second);
			indexBuffer.push_back(first + 1);

			indexBuffer.push_back(second);
			indexBuffer.push_back(second + 1);
			indexBuffer.push_back(first + 1);
		}
	}

	
	Renderer::CreateMesh(
		"Sphere",
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