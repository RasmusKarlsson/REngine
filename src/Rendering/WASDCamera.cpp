///////////////////////////////////////////////////////////
//
// Copyright(c) 2017 Rasmus Karlsson
//
///////////////////////////////////////////////////////////

#pragma once
#include "WASDCamera.h"
#include "MathHelpers.h"
#include <glm/gtc/constants.hpp>

#include "glm/gtc/quaternion.hpp"

WASDCamera::WASDCamera()
{
	m_CameraPosition = vec3();
	m_direction = vec3(0.0f,0.0f,1.0f);
	m_speed = 0.001f;
}

WASDCamera::~WASDCamera()
{

}

void WASDCamera::UpdateCameraState(float dt)
{
	float left = (float)Input::IsPressed(VK_LEFT);
	float right = (float)Input::IsPressed(VK_RIGHT);
	float up = (float)Input::IsPressed(VK_UP);
	float down = (float)Input::IsPressed(VK_DOWN);

	UpdateRotation(1.0f);

	m_direction = vec3(sinf(m_phi) * cosf(m_theta),
												cosf(m_phi),
												sinf(m_phi) * sinf(m_theta));

	m_CameraPosition += m_speed*m_direction*vec3(right - left, 0.0f, up - down);
	m_CameraTarget = m_direction;
}


void WASDCamera::UpdateRotation(float dt)
{
	vec2 mouseDiff = vec2();
	if (Input::IsPressed(MK_LBUTTON))
	{
		if (m_bHaveMousePos)
		{
			vec2 mouseDiff = m_orbitSpeed * (m_currentMousePos - m_lastMousePos);
		}
		m_lastMousePos = m_currentMousePos;
		m_bHaveMousePos = true;
	}
	else if (!Input::IsPressed(MK_RBUTTON))
	{
		m_bHaveMousePos = false;
	}

	m_theta += 1.0f*mouseDiff.x;
	m_phi -= 1.0f*mouseDiff.y;

	if (m_phi < 0.001f) m_phi = 0.001f;
	if (m_phi > pi<float>() - 0.001f) m_phi = pi<float>() - 0.001f;
}

