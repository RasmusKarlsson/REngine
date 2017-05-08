#pragma once
//#include <GL/glew.h>

#include "Camera.h"
#include "Input.h"
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <vector>
//#include <iostream>

using namespace std;
using namespace glm;

class OrbitCamera : public Camera
{
public:
	OrbitCamera();
	~OrbitCamera();

	void UpdateCameraState(float dt) override;
	void SetOrbitDistance(float distance) { m_rho = distance; };

private:

	void UpdateSphericalPosition();
	void UpdateRotation(float dt);
	void UpdateZoom(float dt);
	void UpdatePan(float dt);

	float m_theta;
	float m_phi;
	float m_rho;

	vec2 m_orbitSpeed = vec2(0.01f,0.01f);
	float m_panSpeed = 0.0025f;
	float m_zoomSpeed = 1.0f;
	vec2 m_lastOrbitDiff;

	float m_orbitDecay;
	vec2 m_lastMousePos;
	vec2 m_currentMousePos;
	float m_mouseWheel;
	bool m_bHaveMousePos;

};
