#pragma once
#include <GL/glew.h>

#include "Camera.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>

using namespace std;
using namespace glm;

class WASDCamera : public Camera
{
public:
	WASDCamera();
	~WASDCamera();

	void UpdateCameraState(float dt) override;
	void UpdateRotation(float dt);

private:

	vec3 m_position;
	vec3 m_direction;
	float m_speed;

	float m_theta;
	float m_phi;

	vec2 m_orbitSpeed = vec2(0.01f, 0.01f);
	float m_panSpeed = 0.0025f;
	float m_zoomSpeed = 1.0f;
	vec2 m_lastOrbitDiff;

	float m_orbitDecay;
	vec2 m_lastMousePos;
	vec2 m_currentMousePos;
	float m_mouseWheel;
	bool m_bHaveMousePos;
};
