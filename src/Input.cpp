///////////////////////////////////////////////////////////
//
// Copyright(c) 2017 Rasmus Karlsson
//
///////////////////////////////////////////////////////////

#pragma once
#include "Input.h"
#include <iostream>

///////////////////////////////////////////////////////////

HWND g_hwnd;


bool Input::m_Keys[256] = { false };
bool Input::m_KeysPrev[256] = { false };

int Input::	mouseWheel;

void Input::KeyDown(WPARAM wParam)
{
	m_Keys[wParam] = true;
}

void Input::KeyUp(WPARAM wParam)
{
	m_Keys[wParam] = false;
}

bool Input::IsPressed(WPARAM wParam)
{
	return m_Keys[wParam];
}

bool Input::IsPressedOnce(WPARAM wParam)
{
	bool previousState = m_KeysPrev[wParam];
	m_KeysPrev[wParam] = m_Keys[wParam];
	return (m_KeysPrev[wParam] && !previousState);
}

void Input::ResetKeys()
{
	for (int i = 0; i < maxKeys; i++)
	{
		m_Keys[i] = false;
	}
}

POINT Input::GetMousePos(bool ScreenRelative)
{
	POINT mousePos;
	GetCursorPos(&mousePos);

	//relative to hwnd's client area
	if (ScreenRelative)
	{
		ScreenToClient(g_hwnd, &mousePos);
	}

	return mousePos;
}

vec2 Input::GetMousePosV(bool Normalized, bool ScreenRelative)
{
	POINT mousePos;
	GetCursorPos(&mousePos);

	//relative to hwnd's client area
	if (ScreenRelative)
	{
		ScreenToClient(g_hwnd, &mousePos);
	}

	vec2 mousePosV = vec2(mousePos.x, mousePos.y);
	if (Normalized)
	{
		RECT rect;
		DwmGetWindowAttribute(g_hwnd, DWMWA_EXTENDED_FRAME_BOUNDS, &rect, sizeof(rect));
		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;

		mousePosV.x /= width;
		mousePosV.y /= height;
	}

	return mousePosV;
}

void Input::SetMouseWheelValue(int value)
{
	Input::mouseWheel = value;
}