///////////////////////////////////////////////////////////
//
// Copyright(c) 2017 Rasmus Karlsson
//
///////////////////////////////////////////////////////////

#include "Entity.h"

Entity::Entity()
{
	m_name = "DefaultEntity";
	UpdateMatrices();
}


Entity::~Entity()
{
}

void Entity::Delete()
{
	if (m_vboVertex) {
		glDeleteBuffers(1, &m_vboVertex);
	}
	if (m_vboIndex) {
		glDeleteBuffers(1, &m_vboIndex);
	}
	if (m_vao) {
		glDeleteVertexArrays(1, &m_vao);
	}

	m_vao = 0;
	m_vboVertex = 0;
	m_vboIndex = 0;
}

void Entity::UpdateMatrices()
{
	m_ModelMatrix = translate(mat4(1.0f), m_LocalPosition);

	m_ModelMatrix = rotate(m_ModelMatrix, m_LocalRotation.x, vec3(1, 0, 0));
	m_ModelMatrix = rotate(m_ModelMatrix, m_LocalRotation.y, vec3(0, 1, 0));
	m_ModelMatrix = rotate(m_ModelMatrix, m_LocalRotation.z, vec3(0, 0, 1));

	m_ModelMatrix = scale(m_ModelMatrix, m_LocalScale);

	m_ModelMatrixInv = inverse(m_ModelMatrix);

	if (m_parent != NULL)
	{
		m_WorldMatrix = m_parent->GetWorldMatrix() * m_ModelMatrix;
	}
	else
	{
		m_WorldMatrix = m_ModelMatrix;
	}
	
	m_dirty = false;
}

void Entity::SetDirty()
{
	m_dirty = true;
	for (auto it = begin(m_children); it != end(m_children); ++it) {
		(*it)->SetDirty();
	}
}

void Entity::SetParent(Entity* parent)
{
	mat4 parentMatrixInv = inverse(parent->GetWorldMatrix());
	m_LocalPosition = parentMatrixInv * vec4(m_LocalPosition,1.0);
	m_LocalRotation = parentMatrixInv * vec4(m_LocalRotation, 0.0);
	m_LocalScale	= parentMatrixInv * vec4(m_LocalScale, 0.0);
	SetDirty();

	m_parent = parent;
	parent->AddChild(this);
}


void Entity::AddChild(Entity* child)
{
	//Dont add it twice
	for (auto it = begin(m_children); it != end(m_children); ++it) {
		if ((*it) == child) return;
	}

	if (child->GetParent() == NULL)
		child->SetParent(this);

	m_children.push_back(child);
}

void Entity::SetPosition(float x, float y, float z)
{
	m_LocalPosition.x = x;
	m_LocalPosition.y = y;
	m_LocalPosition.z = z;
	SetDirty();
}

void Entity::SetScale(float x, float y, float z)
{
	m_LocalScale.x = x;
	m_LocalScale.y = y;
	m_LocalScale.z = z;
	SetDirty();
}

void Entity::SetRotation(float x, float y, float z)
{
	m_LocalRotation.x = x;
	m_LocalRotation.y = y;
	m_LocalRotation.z = z;
	SetDirty();
}