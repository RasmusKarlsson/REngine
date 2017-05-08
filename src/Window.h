///////////////////////////////////////////////////////////
//
// Copyright(c) 2017 Rasmus Karlsson
//
///////////////////////////////////////////////////////////

#pragma once

#include <windows.h>
#include <iostream>
#include <string>

#include "Scene.h"

//using namespace std;
//using namespace std::chrono;


class Window {
public:
	LPTSTR windowClass;	// Window Class
	HGLRC RC;			// Rendering Context
	HDC	DC;				// Device Context
	HWND WND;			// Window
	DWORD style;

	string GAME_NAME = "REngine";
	string VERSION	 = "0.0.1";

	struct Config {
		int width;
		int	height;
		int posX;
		int posY;
	} config;

	Window();
	~Window();
	void ShowMessage(LPCSTR message);
	void SetWindowSize(HWND hWnd, int width, int height);
	int Create(HINSTANCE hInstance, int nCmdShow);
	ATOM RegisterClass(HINSTANCE hInstance);
	void Init();
	void Render(double dt);
	void WSwapBuffers();
	void Destroy();

	static LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	Scene* scene;
private:

	GLuint shader;

	double m_updateTimer;

};