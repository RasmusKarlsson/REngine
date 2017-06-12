#pragma once
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <vector>
#include <iostream>

using namespace std;
using namespace glm;

class Camera
{
public:
	Camera();
	~Camera();

	void SetCameraPosition(vec3 position) { m_CameraPosition = position; };
	vec3 GetCameraPosition() { return m_CameraPosition; };
	void SetCameraTarget(vec3 target) { m_CameraTarget = target; };
	vec3 GetCameraTarget() { return m_CameraTarget; };

	vec3 GetCameraDirection() { return normalize(m_CameraTarget - m_CameraPosition); };

	void SetCameraFar(float ffar) { m_far = ffar; };
	float GetCameraFar() { return m_far; };


	void SetViewMatrix(mat4 matrix);
	mat4 GetViewMatrix();

	void SetProjectionMatrix(mat4 matrix);
	mat4 GetProjectionMatrix();

	void CreateProjectionMatrix(float fov, float aspect, float fnear, float ffar);
	void CreateLookAtMatrix();

	virtual void UpdateCameraState(float dt) { return; };
	virtual void SetOrbitDistance(float distance) { return; };

protected:

	vec3 m_CameraPosition;
	vec3 m_CameraTarget;

	mat4 m_ProjectionMatrix;
	mat4 m_ViewMatrix;
	mat4 m_ViewProjectionMatrix;

	float m_far;

};
