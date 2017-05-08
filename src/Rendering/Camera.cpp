///////////////////////////////////////////////////////////
//
// Copyright(c) 2017 Rasmus Karlsson
//
///////////////////////////////////////////////////////////

#pragma once
#include "Camera.h"

Camera::Camera()
{
	
}

Camera::~Camera()
{

}

void Camera::SetViewMatrix(mat4 matrix)
{
	m_ViewMatrix = matrix;
}

mat4 Camera::GetViewMatrix()
{
	return m_ViewMatrix;
}

void Camera::SetProjectionMatrix(mat4 matrix)
{
	m_ProjectionMatrix = matrix;
}

mat4 Camera::GetProjectionMatrix()
{
	return m_ProjectionMatrix;
}

void Camera::CreateProjectionMatrix(float fov, float aspect, float fnear, float ffar)
{
	m_ProjectionMatrix = perspective(fov, aspect, fnear, ffar);
}

void Camera::CreateLookAtMatrix()
{
	m_ViewMatrix = lookAt(m_CameraPosition, m_CameraTarget, vec3(0.0f,1.0f,0.0f));
}