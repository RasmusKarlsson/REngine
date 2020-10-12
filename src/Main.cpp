///////////////////////////////////////////////////////////
//
// Copyright(c) 2017 Rasmus Karlsson
//
///////////////////////////////////////////////////////////

#include "Main.h"
#include "Window.h"
#include <chrono>
#include <algorithm>

#include "Shader.h"

typedef std::chrono::duration<long, std::ratio<1, 60>> sixtieths_of_a_sec;
constexpr auto kMaxDeltatime = sixtieths_of_a_sec{ 1 };
static const double s_updateRate = 600000.0;

Window window;


using namespace std;

const unsigned long maxPathSize = 260;

static wstring fromUTF8(const char* path) {
	wchar_t buf[maxPathSize];
	size_t result = MultiByteToWideChar(CP_UTF8, 0, path, strlen(path), buf, ARRAYSIZE(buf));
	return wstring(buf, result);
}

static string toUTF8(const wchar_t* path, size_t length) {
	char buf[maxPathSize];
	size_t result = WideCharToMultiByte(CP_UTF8, 0, path, length, buf, sizeof(buf), NULL, NULL);
	return string(buf, result);
}

DWORD WINAPI ShaderDirectoryUpdate(void *data) {
	char dir[maxPathSize];
	GetCurrentDirectory(maxPathSize, dir);
	//cout << dir << endl;

	HANDLE h = CreateFileW(fromUTF8(dir).c_str(), FILE_LIST_DIRECTORY, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);

	if (h == INVALID_HANDLE_VALUE)
		return false;

	char buf[65536];
	DWORD bufsize = 0;

	unsigned int filter = FILE_NOTIFY_CHANGE_FILE_NAME;

	while (ReadDirectoryChangesW(h, buf, sizeof(buf), true, filter, &bufsize, NULL, NULL)) {
		if (bufsize == 0)
			continue;

		size_t offset = 0;

		for (;;) {
			FILE_NOTIFY_INFORMATION* file = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(buf + offset);

			//if (file->Action == FILE_ACTION_RENAMED_NEW_NAME)
			{
				string filepath = toUTF8(file->FileName, file->FileNameLength / sizeof(WCHAR));

				if (Texture::IsImageFileExtension(filepath))
				{
					Texture::SetTextureDirty(filepath);
				}
				else if(Shader::IsShaderFileExtension(filepath))
				{
					Shader::SetShaderDirty(filepath);
				}
			}

			if (!file->NextEntryOffset)
				break;

			offset += file->NextEntryOffset;
		}
	}

	CloseHandle(h);

	return {};
}

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {

	if (window.Create(hInstance, nCmdShow) != 0) {
		PostQuitMessage(1);
	}

	window.Init();

	//Create a thread that listens to changes in the shader directory
	CreateThread(NULL, 0, ShaderDirectoryUpdate, NULL, 0, NULL);

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
