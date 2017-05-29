#pragma once

#include <gl\glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "Material.h"
#include <vector>

using namespace glm;

class BBox
{
public:
	BBox() {};
	BBox(vec3 min, vec3 max) { SetBoundingBox(min, max); };
	~BBox() {};

	void SetBoundingBox(vec3 min, vec3 max) {
		m_min = min;
		m_max = max;
	}
	vec3 GetBoundingBoxMin() { return m_min; };
	vec3 GetBoundingBoxMax() { return m_max; };

	vec3 GetBoundingScale() { return m_max - m_min; };
	vec3 GetBoundingPosition() { return (m_max + m_min)/2.0f; };
	/*
	bool RayIntersection(ray r) {
	double tx1 = (b.min.x - r.x0.x)*r.n_inv.x;
	double tx2 = (b.max.x - r.x0.x)*r.n_inv.x;

	double tmin = min(tx1, tx2);
	double tmax = max(tx1, tx2);

	double ty1 = (b.min.y - r.x0.y)*r.n_inv.y;
	double ty2 = (b.max.y - r.x0.y)*r.n_inv.y;

	tmin = glm::max(tmin, min(ty1, ty2));
	tmax = min(tmax, max(ty1, ty2));

	return tmax >= tmin;
	}*/
	
	vec3 m_max = vec3(1.0);
	vec3 m_min = vec3(0.0);
};

class Entity
{
public:

	Entity();
	~Entity();

	enum RENDERSTYLE
	{
		RENDERSTYLE_STANDARD = 0,
		RENDERSTYLE_2D,
		RENDERSTYLE_OPACITY,
		RENDERSTYLE_ADD,
		RENDERSTYLE_STANDARD_WIRE,
		RENDERSTYLE_2D_WIRE,
	};

	void Delete();

	void SetName(string name) { m_name = name; };
	string GetName() { return m_name; };

	mat4 GetWorldMatrix()		{ if (IsDirty()) UpdateMatrices(); return m_WorldMatrix; };
	mat4 GetInvModelMatrix()	{ return m_ModelMatrixInv; };
	vec3 GetPosition()			{ return m_LocalPosition; };
	vec3 GetScale()				{ return m_LocalScale; };
	vec3 GetRotation()			{ return m_LocalRotation; };

	vec3 GetWorldPosition()		{ return vec3(m_WorldMatrix[3]); };
	vec3 GetWorldScale();
	vec3 GetWorldRotation();


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

	void AddPosition(vec3 position) { m_LocalPosition += position;	SetDirty(); };
	void AddScale(vec3 scale) { m_LocalScale += scale;			SetDirty(); };
	void AddRotation(vec3 rotation) { m_LocalRotation += rotation;	SetDirty(); };

	void MulScale(vec3 scale) { m_LocalScale *= scale;			SetDirty(); };

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

	void SetRenderStyle(int renderStyle) { m_renderStyle = renderStyle; };
	int GetRenderStyle() { return m_renderStyle; };

	void SetBoundingBox(BBox* bbox) { m_bbox = bbox; };
	BBox* GetBoundingBox() { return m_bbox; };

	BBox* m_bbox;

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
	int m_renderStyle = 0;
};

