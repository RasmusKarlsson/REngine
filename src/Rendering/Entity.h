#pragma once

#include <gl\glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Material.h"
#include <vector>

using namespace glm;

class Entity
{
public:
	Entity();
	~Entity();

	void Delete();

	void SetName(string name) { m_name = name; };
	string GetName() { return m_name; };

	mat4 GetWorldMatrix() { if (IsDirty()) UpdateMatrices(); return m_WorldMatrix; };
	mat4 GetInvModelMatrix()	{ return m_ModelMatrixInv; };
	vec3 GetPosition()			{ return m_LocalPosition; };
	vec3 GetScale()				{ return m_LocalScale; };
	vec3 GetRotation()			{ return m_LocalRotation; };

	GLuint GetVao()				{ return m_vao; };
	GLuint GetVboVertex()		{ return m_vboVertex; };
	GLuint GetVboIndex()		{ return m_vboIndex; };
	GLuint GetSize()			{ return m_triangleSize; };
	GLuint GetShader()			{ return m_shader; };

	void SetPosition(vec3 position)		{ m_LocalPosition = position;	SetDirty(); };
	void SetScale(vec3 scale)			{ m_LocalScale = scale;			SetDirty();	};
	void SetRotation(vec3 rotation)		{ m_LocalRotation = rotation;	SetDirty();	};

	void SetPosition(float x, float y, float z);
	void SetScale	(float x, float y, float z);
	void SetRotation(float x, float y, float z);

	void UpdateMatrices();
	bool IsDirty() { return m_dirty; };
	void SetDirty();
	bool m_dirty = true;

	void SetParent(Entity* parent);
	Entity* GetParent() { return m_parent; };
	void AddChild(Entity* child);

	void SetMaterial(Material* material) { m_material = material; };
	Material* GetMaterial() { return m_material; };

protected:

	string m_name;
	mat4 m_WorldMatrix	  = mat4();

	mat4 m_ModelMatrix	  = mat4();
	mat4 m_ModelMatrixInv = mat4();
	vec3 m_LocalPosition  = vec3(0.0f);
	vec3 m_LocalScale	  = vec3(1.0f);
	vec3 m_LocalRotation  = vec3(0.0f);

	Entity* m_parent = NULL;
	std::vector<Entity*> m_children;

	GLuint 
		m_vao, 
		m_vboVertex, 
		m_vboIndex, 
		m_triangleSize,
		m_shader;

	Material* m_material;
};

