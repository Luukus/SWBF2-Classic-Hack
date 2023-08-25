#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <cstdint>
#include <iostream>
#include <thread>

#include "context.h"
#include "shared/hooks/hooks.h"
#include "hack.h"

bool Setup(HackContext* context) {
	// Setup gui
	try {
		gui::Setup();
	}
	catch (const std::exception& error) {
		MessageBeep(MB_ICONERROR);
		MessageBox(0, error.what(), "Gui Setup Error", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	// Setup hooks and pass in render func
	try {
		hooks::Setup(context, hack::InitGui, hack::Render);
	}
	catch (const std::exception& error) {
		gui::Destroy(); // Destroy gui since it was created
		MessageBeep(MB_ICONERROR);
		MessageBox(0, error.what(), "Hook Setup Error", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	return true;
}

void init() {
	HackContext* context = new HackContext();
	if (Setup(context)) {

		FILE* f = nullptr;
		if (context->showConsole) {
			// Create console
			AllocConsole();
			freopen_s(&f, "CONOUT$", "w", stdout);
		}

		hack::Init(context);
		hooks::EnableHooks();

		while (!GetAsyncKeyState(VK_END))
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			hack::Update(context, &gui::open, context->windowWidth, context->windowHeight);
		}

		hack::Destroy(context);

		// Cleanup
		hooks::Destroy();
		gui::Destroy();

		if (context->showConsole) {
			fclose(f);
			FreeConsole();
		}
	}
}

void mainThread(const HMODULE instance) noexcept {
	try {
		init();
	}
	catch (const std::exception& error) {
		MessageBeep(MB_ICONERROR);
		MessageBox(0, error.what(), "Hack Error", MB_OK | MB_ICONEXCLAMATION);
	}

	FreeLibraryAndExitThread(instance, 0);
}

int __stdcall DllMain(const HMODULE instance,const std::uintptr_t reason,const void* reserved) {
	if (reason == DLL_PROCESS_ATTACH) {
		DisableThreadLibraryCalls(instance);

		// create hack thread
		const auto thread = CreateThread(
			nullptr,
			0,
			reinterpret_cast<LPTHREAD_START_ROUTINE>(mainThread),
			instance,
			0,
			nullptr
		);

		if (thread)
			CloseHandle(thread);
	}

	return 1;
}