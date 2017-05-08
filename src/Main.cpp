///////////////////////////////////////////////////////////
//
// Copyright(c) 2017 Rasmus Karlsson
//
///////////////////////////////////////////////////////////

#include <gl\glew.h>
#include "Main.h"
#include "Window.h"
#include <chrono>
#include <algorithm>
//#include "Scene.h"

typedef std::chrono::duration<long, std::ratio<1, 60>> sixtieths_of_a_sec;
constexpr auto kMaxDeltatime = sixtieths_of_a_sec{ 1 };

Window window;

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {

	if (window.Create(hInstance, nCmdShow) != 0) {
		PostQuitMessage(1);
	}

	window.Init();

	MSG msg;
	bool active = true;

	auto lastEndTime = std::chrono::high_resolution_clock::now();

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

		//typedef std::common_type<decltype(frameTime), decltype(kMaxDeltatime)>::type common_duration;
	//	auto mDeltaTime = std::min<common_duration>(frameTime, kMaxDeltatime);

		//if (frameTime > std::chrono::milliseconds(0)) { continue; }

		long microseconds = std::chrono::duration_cast<std::chrono::duration<long, std::micro>>(frameTime).count();
		double dt = microseconds / 1000000.0;
	//	printf("%lf \n", dt);
		window.Render(dt);
		window.WSwapBuffers();
		lastEndTime = newEndTime;
	}
	window.Destroy();

	return (int)msg.wParam;
}
