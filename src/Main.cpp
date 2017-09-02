///////////////////////////////////////////////////////////
//
// Copyright(c) 2017 Rasmus Karlsson
//
///////////////////////////////////////////////////////////

#include "Main.h"
#include "Window.h"
#include <chrono>
#include <algorithm>

typedef std::chrono::duration<long, std::ratio<1, 60>> sixtieths_of_a_sec;
constexpr auto kMaxDeltatime = sixtieths_of_a_sec{ 1 };
static const double s_updateRate = 600000.0;

Window window;

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {

	if (window.Create(hInstance, nCmdShow) != 0) {
		PostQuitMessage(1);
	}

	window.Init();

	MSG msg;
	bool active = true;

	auto lastEndTime = std::chrono::high_resolution_clock::now();
	double m_updateTimer = 0.0;

	while (active) {
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				active = false;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		auto newEndTime = std::chrono::high_resolution_clock::now();
		auto frameTime = newEndTime - lastEndTime;

		long microseconds = std::chrono::duration_cast<std::chrono::duration<long, std::micro>>(frameTime).count();
		double dt = microseconds / 1000000.0;
		m_updateTimer += dt;
		if (m_updateTimer > 1.0 / s_updateRate)
		{
			window.Render(m_updateTimer);
			window.WSwapBuffers();
			lastEndTime = newEndTime;
			m_updateTimer = 0.0;
		}
	}
	window.Destroy();

	return (int)msg.wParam;
}
