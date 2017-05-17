#pragma once

#include <gl\glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Material.h"
#include <vector>

using namespace glm;

class BBox
{
public:
	BBox() {};
	~BBox() {};

	void SetBoundingBox(vec3 min, vec3 max) {
		min = min;
		max = max;
	}
	vec3 GetBoundingBoxMin() { return min; };
	vec3 GetBoundingBoxMax() { return max; };
	/*
	bool RayIntersection(ray r) {
		double tx1 = (b.min.x - r.x0.x)*r.n_inv.x;
		double tx2 = (b.max.x - r.x0.x)*r.n_inv.x;

		double tmin = min(tx1, tx2);
		double tmax = max(tx1, tx2);

		double ty1 = (b.min.y - r.x0.y)*r.n_inv.y;
		double ty2 = (b.max.y - r.x0.y)*r.n_inv.y;

		tmin = max(tmin, min(ty1, ty2));
		tmax = min(tmax, max(ty1, ty2));

		return tmax >= tmin;
	}
	*/
	vec3 max = vec3();
	vec3 min = vec3();
};

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

	bool IsDead() { return m_dead; };
	void Kill() { m_dead = true; };

	bool m_dead = false;

	void SetParent(Entity* parent);
	Entity* GetParent() { return m_parent; };
	void AddChild(Entity* child);

	void SetMaterial(Material* material) { m_material = material; };
	Material* GetMaterial() { return m_material; };

	BBox m_bbox;

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

