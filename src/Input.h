///////////////////////////////////////////////////////////
//
// Copyright(c) 2017 Rasmus Karlsson
//
///////////////////////////////////////////////////////////

#pragma once
#include <windows.h>
#include <Dwmapi.h>
#include <glm\glm.hpp>

const unsigned int maxKeys = 256;
using namespace glm;

class Input {

public:
	
	static void KeyDown(WPARAM wParam);
	static void KeyUp(WPARAM wParam);
	static bool IsPressed(WPARAM wParam);
	static void ResetKeys();

	static bool m_Keys[maxKeys];

	static POINT GetMousePos(bool ScreenRelative = true);

	static vec2 Input::GetMousePosV(bool Normalized = false, bool ScreenRelative = true);

	//static POINT m_mousePos;
	static void SetMouseWheelValue(int value);
	static int GetMouseWheel() {return mouseWheel;};

	static int mouseWheel;

};
