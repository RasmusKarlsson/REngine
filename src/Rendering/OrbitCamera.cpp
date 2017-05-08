///////////////////////////////////////////////////////////
//
// Copyright(c) 2017 Rasmus Karlsson
//
///////////////////////////////////////////////////////////

#pragma once
#include "OrbitCamera.h"
#include "MathHelpers.h"
#include <glm/gtc/constants.hpp>

#include "glm/gtc/quaternion.hpp"

OrbitCamera::OrbitCamera()
{
	m_theta = half_pi<float>();
	m_phi = half_pi<float>();
	m_rho = 1.0f;
	m_mouseWheel = 0.0f;
	m_bHaveMousePos = false;

	m_lastOrbitDiff = vec2(0.0f,0.0f);
}

OrbitCamera::~OrbitCamera()
{

}

void OrbitCamera::UpdateCameraState(float dt)
{
	m_currentMousePos = Input::GetMousePosV(false, true);

	UpdatePan(dt);
	UpdateZoom(dt);
	UpdateRotation(dt);
	UpdateSphericalPosition();
}

void OrbitCamera::UpdateSphericalPosition()
{
	m_CameraPosition = m_CameraTarget + vec3(m_rho * sinf(m_phi) * cosf(m_theta),
											 m_rho * cosf(m_phi),
											 m_rho * sinf(m_phi) * sinf(m_theta));
}

void OrbitCamera::UpdateRotation(float dt)
{
	m_orbitDecay = MathHelpers::Lerp(m_orbitDecay, 0.0f, 10.0*dt);
	if (Input::IsPressed(MK_LBUTTON))
	{
		if (m_bHaveMousePos)
		{
			vec2 mouseDiff = m_orbitSpeed * (m_currentMousePos - m_lastMousePos);

			m_orbitDecay = 1.0f;
			m_lastOrbitDiff = mouseDiff;
		}
		m_lastMousePos = m_currentMousePos;
		m_bHaveMousePos = true;
	}
	else if(!Input::IsPressed(MK_RBUTTON))
	{
		m_bHaveMousePos = false;
	}

	m_theta += m_orbitDecay*m_lastOrbitDiff.x;
	m_phi -= m_orbitDecay*m_lastOrbitDiff.y;

	if (m_phi < 0.001f) m_phi = 0.001f;
	if (m_phi > pi<float>() - 0.001f) m_phi = pi<float>() - 0.001f;
}

void OrbitCamera::UpdatePan(float dt)
{
	if (Input::IsPressed(MK_RBUTTON))
	{
		if (m_bHaveMousePos)
		{
			vec2 mouseDiff = m_rho * m_panSpeed * (m_currentMousePos - m_lastMousePos);
			vec3 atVector = normalize(m_CameraPosition - m_CameraTarget);
			vec3 crossAtUp = normalize(cross(atVector, vec3(0.0f, 1.0f, 0.0f)));
			vec3 crossAtLeft = normalize(cross(atVector, crossAtUp));
			m_CameraTarget += mouseDiff.x * crossAtUp;
			m_CameraTarget -= mouseDiff.y * crossAtLeft;
		}
		m_lastMousePos = m_currentMousePos;
		m_bHaveMousePos = true;
	}
	else if (!Input::IsPressed(MK_LBUTTON))
	{
		m_bHaveMousePos = false;
	}
}


void OrbitCamera::UpdateZoom(float dt)
{
	int mouseWheel = Input::GetMouseWheel() / 120;
	m_mouseWheel = MathHelpers::Lerp(m_mouseWheel, (float)mouseWheel, 10.0*dt);

	if (m_rho < 2.0f)
	{
		m_CameraTarget -= (m_zoomSpeed * m_mouseWheel) * normalize(m_CameraPosition - m_CameraTarget);
	}
	else
	{
		m_rho -= m_zoomSpeed * m_mouseWheel;
	}
		
}

