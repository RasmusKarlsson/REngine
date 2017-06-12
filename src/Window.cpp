///////////////////////////////////////////////////////////
//
// Copyright(c) 2017 Rasmus Karlsson
//
///////////////////////////////////////////////////////////

//#include "Input.h"
#include "windows.h"
#include "Shellapi.h"
#include <string>
#include <atlstr.h>
#include "Window.h"
#include "gl/glew.h"
#include "gl/glext.h"
#include "gl/wglext.h"

//#include "Renderer.h"
#include "Quad.h"
#include "Cube.h"
#include "Sphere.h"
#include "Input.h"

extern "C"
{
	//Forces computers with 2 Gpus to use the performant one.
	__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}


///////////////////////////////////////////////////////////

extern double timeElapsed = 0.0;
extern int SCREEN_WIDTH = 1280;
extern int SCREEN_HEIGHT = 720;

extern bool DEFERRED_RENDERING = true;

extern HWND g_hwnd;
unsigned int g_bits;
RECT g_rect;
long g_extStyle;
long g_style;
DEVMODE g_screenSettings;

static int g_glwin_fullscreen = 0;

Window *staticWindow;

bool glwin_enter_fullscreen(int width, int height);
bool glwin_leave_fullscreen(int width, int height);

Window::Window() {
	config.width = SCREEN_WIDTH;
	config.height = SCREEN_HEIGHT;
	config.posX = CW_USEDEFAULT;
	config.posY = 0;
	style = WS_CAPTION | WS_SYSMENU | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	staticWindow = this;
}

///////////////////////////////////////////////////////////

Window::~Window() {
}

///////////////////////////////////////////////////////////

void Window::ShowMessage(LPCSTR message) {
	MessageBox(0, message, "Window::create", MB_ICONERROR);
}

void Window::SetWindowSize(HWND hWnd, int width, int height)
{
	if (IsWindow(hWnd))
	{
		DWORD dwStyle = GetWindowLongPtr(hWnd, GWL_STYLE);
		DWORD dwExStyle = GetWindowLongPtr(hWnd, GWL_EXSTYLE);
		HMENU menu = GetMenu(hWnd);

		RECT rc = { 0, 0, width, height };

		AdjustWindowRectEx(&rc, dwStyle, menu ? TRUE : FALSE, dwExStyle);

		SetWindowPos(hWnd, NULL, 0, 0, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_NOMOVE);
		glViewport(0, 0, width, height);
	}
}

///////////////////////////////////////////////////////////

inline int printOglError(int line)
{

	GLenum glErr;
	int    retCode = 0;

	glErr = glGetError();
	if (glErr != GL_NO_ERROR)
	{
		printf("glError in file %d @ %d\n", line, glErr);
		retCode = 1;
	}
	return retCode;
}
int Window::Create(HINSTANCE hInstance, int nCmdShow) {

	AllocConsole();

	

	freopen("CONOUT$", "w", stdout);

	windowClass = MAKEINTATOM(RegisterClass(hInstance));
	if (windowClass == 0) {
		ShowMessage("registerClass() failed.");
		return 1;
	}

	// create temporary window

	HWND fakeWND = CreateWindow(
		windowClass, "Fake Window",
		style,
		0, 0,						// position x, y
		1, 1,						// width, height
		NULL, NULL,					// parent window, menu
		hInstance, NULL);			// instance, param

	HDC fakeDC = GetDC(fakeWND);	// Device Context

	PIXELFORMATDESCRIPTOR fakePFD;
	ZeroMemory(&fakePFD, sizeof(fakePFD));
	fakePFD.nSize = sizeof(fakePFD);
	fakePFD.nVersion = 1;
	fakePFD.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	fakePFD.iPixelType = PFD_TYPE_RGBA;
	fakePFD.cColorBits = 32;
	fakePFD.cAlphaBits = 8;
	fakePFD.cDepthBits = 24;

	int fakePFDID = ChoosePixelFormat(fakeDC, &fakePFD);
	if (fakePFDID == 0) {
		ShowMessage("ChoosePixelFormat() failed.");
		return 1;
	}

	if (SetPixelFormat(fakeDC, fakePFDID, &fakePFD) == false) {
		ShowMessage("SetPixelFormat() failed.");
		return 1;
	}

	HGLRC fakeRC = wglCreateContext(fakeDC);	// Rendering Contex

	if (fakeRC == 0) {
		ShowMessage("wglCreateContex() failed.");
		return 1;
	}

	if (wglMakeCurrent(fakeDC, fakeRC) == false) {
		ShowMessage("wglMakeCurrent() failed.");
		return 1;
	}

	// get pointers to functions (or init opengl loader here)

	PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = nullptr;
	wglChoosePixelFormatARB = reinterpret_cast<PFNWGLCHOOSEPIXELFORMATARBPROC>(wglGetProcAddress("wglChoosePixelFormatARB"));
	if (wglChoosePixelFormatARB == nullptr) {
		ShowMessage("wglGetProcAddress() failed.");
		return 1;
	}

	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = nullptr;
	wglCreateContextAttribsARB = reinterpret_cast<PFNWGLCREATECONTEXTATTRIBSARBPROC>(wglGetProcAddress("wglCreateContextAttribsARB"));
	if (wglCreateContextAttribsARB == nullptr) {
		ShowMessage("wglGetProcAddress() failed.");
		return 1;
	}

	// center the window (optional)

	RECT primaryDisplaySize;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &primaryDisplaySize, 0);	// system taskbar and application desktop toolbars not included
	config.posX = (primaryDisplaySize.right - config.width) / 2;
	config.posY = (primaryDisplaySize.bottom - config.height) / 2;

	// create new window and context
								
	WND = CreateWindow(
		windowClass, "OpenGL Window",	// class name, window name
		style,							// styles
		config.posX, config.posY,		// posx, posy. If x is set to CW_USEDEFAULT y is ignored
		config.width, config.height,	// width, height
		NULL, NULL,						// parent window, menu
		hInstance, NULL);				// instance, param

	SetWindowSize(WND,config.width, config.height);

	//Make a global instance
	g_hwnd = WND;

	DC = GetDC(WND);

	const int pixelAttribs[] = {
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
		WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
		WGL_COLOR_BITS_ARB, 32,
		WGL_ALPHA_BITS_ARB, 8,
		WGL_DEPTH_BITS_ARB, 24,
		WGL_STENCIL_BITS_ARB, 8,
		WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
		WGL_SAMPLES_ARB, 4,
		0
	};

	int pixelFormatID; UINT numFormats;
	int status = wglChoosePixelFormatARB(DC, pixelAttribs, NULL, 1, &pixelFormatID, &numFormats);

	if (status == FALSE || numFormats == 0) {
		ShowMessage("wglChoosePixelFormatARB() failed.");
		return 1;
	}

	PIXELFORMATDESCRIPTOR PFD;
	DescribePixelFormat(DC, pixelFormatID, sizeof(PFD), &PFD);
	SetPixelFormat(DC, pixelFormatID, &PFD);

	const int major_min = 4, minor_min = 0;
	int  contextAttribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, major_min,
		WGL_CONTEXT_MINOR_VERSION_ARB, minor_min,
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
//		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
		0
	};

	RC = wglCreateContextAttribsARB(DC, 0, contextAttribs);
	if (RC == NULL) {
		ShowMessage("wglCreateContextAttribsARB() failed.");
		return 1;
	}

	// delete temporary context and window

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(fakeRC);
	ReleaseDC(fakeWND, fakeDC);
	DestroyWindow(fakeWND);
	if (!wglMakeCurrent(DC, RC)) {
		ShowMessage("wglMakeCurrent() failed.");
		return 1;
	}

	// init opengl loader here (extra safe version)
	glewExperimental = GL_TRUE;
	glewInit();

	printOglError(0);
	SetWindowText(WND, (LPCSTR)glGetString(GL_VERSION));
	ShowWindow(WND, nCmdShow);
	
	return 0;
}

///////////////////////////////////////////////////////////

ATOM Window::RegisterClass(HINSTANCE hInstance) {

	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(wcex));
	wcex.cbSize = sizeof(wcex);
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc = WindowProcedure;
	wcex.hInstance = hInstance;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.lpszClassName = "Core";

	return RegisterClassEx(&wcex);
}

///////////////////////////////////////////////////////////


void Window::Init() {

	ShowCursor(FALSE);
	DragAcceptFiles(g_hwnd,TRUE);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
//	Renderer::SetClearColor(vec4(0.2f, 0.4f, 0.6f, 1.0f));


	scene = new Scene();
}

void Window::Render(double dt) {
	timeElapsed += dt;
	scene->UpdateScene((float)dt);
	scene->RenderScene((float)dt);

	if (dt > 0.0 && dt < 0.1)
	{
		static double frametimes = 0.0;
		static int frames = 0;
		static double fps = 60.0;
		frametimes += dt;
		frames++;
		if (frames > 100)
		{
			fps = 100.0 / frametimes;
			frames = 1;
			frametimes = 0.0;
			char buffer[100];
			snprintf(buffer, 255, "%s %s - [FPS: %3.2f]", "REngine", "0.0.1", (float)fps);
			SetWindowText(WND, buffer);
		}
		
	}
}

///////////////////////////////////////////////////////////

void Window::WSwapBuffers() {
	SwapBuffers(DC);
}

///////////////////////////////////////////////////////////

void Window::Destroy() {
	ShowCursor(TRUE);
	wglMakeCurrent(NULL, NULL);
	if (RC) {
		wglDeleteContext(RC);
	}
	if (DC) {
		ReleaseDC(WND, DC);
	}
	if (WND) {
		DestroyWindow(WND);
	}
}

///////////////////////////////////////////////////////////

LRESULT CALLBACK Window::WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

	switch (message) {
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) {
			PostQuitMessage(0);
		}

		Input::KeyDown(wParam);
		if (wParam == VK_F1)
		{
			glwin_enter_fullscreen(1920, 1200);
			//staticWindow->SetWindowSize(hWnd,staticWindow->config.width, staticWindow->config.height);
			break;
		}
		if (wParam == VK_F2)
		{
			//staticWindow->SetWindowSize(hWnd,staticWindow->config.width*2, staticWindow->config.height*2);
			glwin_leave_fullscreen(1280, 720);
			break;
		}
		if (wParam == VK_F3)
		{
			break;
		}
		break;

	case WM_LBUTTONUP:
		Input::KeyUp(MK_LBUTTON);
		break;
	case WM_LBUTTONDOWN:
		Input::KeyDown(MK_LBUTTON);
		break;
	case WM_RBUTTONUP:
		Input::KeyUp(MK_RBUTTON);
		break;
	case WM_RBUTTONDOWN:
		Input::KeyDown(MK_RBUTTON);
		break;
	case WM_MOUSEWHEEL:
		Input::SetMouseWheelValue(GET_WHEEL_DELTA_WPARAM(wParam));
		break;



	case WM_KEYUP:
		Input::KeyUp(wParam);
		break;

	case WM_KILLFOCUS:
		Input::ResetKeys();
		break;
	case WM_CLOSE:
		PostQuitMessage(0);

		break;

	case WM_INPUT:
	{
		UINT dwSize = 40;
		static BYTE lpb[40];

		GetRawInputData((HRAWINPUT)lParam, RID_INPUT,
			lpb, &dwSize, sizeof(RAWINPUTHEADER));

		RAWINPUT* raw = (RAWINPUT*)lpb;

		if (raw->header.dwType == RIM_TYPEMOUSE)
		{
			int xPosRelative = raw->data.mouse.lLastX;
			int yPosRelative = raw->data.mouse.lLastY;
			//GameState::Instance()->MoveCursor(xPosRelative, yPosRelative);
			printf("Rel %d \n", xPosRelative);
			RECT rect;
			//resetting cursor to stay in center
			if (GetWindowRect(g_hwnd, &rect))
				SetCursorPos((rect.right - rect.left) / 2, (rect.bottom - rect.top) / 2);
		}
		break;
	}
	case WM_DROPFILES:
	{
		HDROP hDropInfo = (HDROP)wParam;
		UINT nNumOfFiles = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, NULL);
		if (nNumOfFiles > 0)
		{
			for (int i = 0; i < nNumOfFiles; ++i)
			{
				CString strFile;
				UINT nFilenameSize = DragQueryFile(hDropInfo, i, NULL, NULL);
				DragQueryFile(hDropInfo, i, strFile.GetBuffer(nFilenameSize + 1), nFilenameSize + 1);
				strFile.ReleaseBuffer();
				printf("FilePath: %s \n", strFile);
				// strFile contains the full path...
			}
		}
		break;
	}

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;		// message handled
}

bool glwin_enter_fullscreen(int width, int height)
{
	DEVMODE screen;
	DWORD style = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP;
	DWORD exstyle = 0;

	if (g_glwin_fullscreen) return true;
	memset(&screen, 0, sizeof(screen));
	screen.dmSize = sizeof(screen);
	screen.dmPelsWidth = width;
	screen.dmPelsHeight = height;
	screen.dmBitsPerPel = 32;
	screen.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

//	if (ChangeDisplaySettings(&screen, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) return false;
	SetWindowLong(g_hwnd, GWL_EXSTYLE, exstyle);
	SetWindowLong(g_hwnd, GWL_STYLE, style);
	SetParent(g_hwnd, NULL);
	SetWindowPos(g_hwnd, HWND_TOP, 0, 0, width, height, SWP_SHOWWINDOW);
	g_glwin_fullscreen = 1;
	glViewport(0, 0, width, height);
	return true;
}

bool glwin_leave_fullscreen(int width, int height)
{
	DEVMODE screen;
	DWORD exstyle = WS_EX_STATICEDGE;
	DWORD style = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!g_glwin_fullscreen) return true;

	RECT rect = { 0,0,(long)width,(long)height };

	g_extStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	g_style = WS_OVERLAPPEDWINDOW;
	if (g_hwnd) {
		g_style |= GetWindowLong(g_hwnd, GWL_STYLE);
		g_extStyle |= GetWindowLong(g_hwnd, GWL_EXSTYLE);
		SetWindowLong(g_hwnd, GWL_EXSTYLE, g_extStyle);
		SetWindowLong(g_hwnd, GWL_STYLE, g_style);
	}
	AdjustWindowRectEx(&rect, g_style, FALSE, g_extStyle);
	if (ChangeDisplaySettings(NULL, 0)
		!= DISP_CHANGE_SUCCESSFUL) {
		g_hwnd = NULL;
	}
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
//	RECT desktop;
	HRESULT stat = DwmGetWindowAttribute(g_hwnd,DWMWA_EXTENDED_FRAME_BOUNDS,&desktop,sizeof(desktop));
	int posX = (desktop.right - desktop.left) / 2 - width / 2;
	int posY = (desktop.bottom - desktop.top) / 2 - height / 2;
//	printf()
	//int border_thickness = GetSystemMetrics(SM_CYCAPTION)+ GetSystemMetrics(SM_CXSIZEFRAME);;

//	SetWindowLong(g_hwnd, GWL_EXSTYLE, exstyle);
//	SetWindowLong(g_hwnd, GWL_STYLE, style);
	SetWindowPos(g_hwnd, HWND_TOP, posX, posY, width, height, SWP_SHOWWINDOW);
	glViewport(0, 0, width, height);
	g_glwin_fullscreen = 0;
	return true;
}
