#include <stdexcept>

#include "gui.h"
#include "menu/menu.h"

// COPIED FROM imgui_impl_win32.h
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND window,
	UINT message,
	WPARAM wideParam,
	LPARAM longParam
);

LRESULT CALLBACK WindowProcess(
	HWND window, 
	UINT message, 
	WPARAM wideParam, 
	LPARAM longParam
);

bool gui::SetupWindowClass(const char* windowClassName) noexcept {
	// init window class
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = DefWindowProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = GetModuleHandle(NULL);
	windowClass.hIcon = NULL;
	windowClass.hCursor = NULL;
	windowClass.hbrBackground = NULL;
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = windowClassName;
	windowClass.hIconSm = NULL;

	// register
	if (!RegisterClassEx(&windowClass)) {
		return false;
	}
	return true;
}

void gui::DestroyWindowClass() noexcept {
	UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
}

bool gui::SetupWindow(const char* windowName) noexcept {
	// Temporary dummy window
	window = CreateWindow(
		windowClass.lpszClassName,
		windowName,
		WS_OVERLAPPEDWINDOW,
		0,
		0,
		100,
		100,
		0,
		0,
		windowClass.hInstance,
		0
	);

	if (!window) {
		return false;
	}
	return true;
}

void gui::DestroyWindow() noexcept {
	if (window) {
		DestroyWindow(window);
	}
}

bool gui::SetupDirectX() noexcept {
	const auto handle = GetModuleHandle("d3d9.dll");
	if (!handle) {
		return false;
	}
	
	// This function is defined and exported by d3d9.dll, so grab it
	using CreateFn = LPDIRECT3D9(__stdcall*)(UINT);
	const auto create = reinterpret_cast<CreateFn>(GetProcAddress(handle, "Direct3DCreate9"));

	if (!create) {
		return false;
	}

	// create d3d9
	d3d9 = create(D3D_SDK_VERSION);
	if (!d3d9) {
		return false;
	}

	// Define the D3D9 device params
	D3DPRESENT_PARAMETERS params = {};
	params.BackBufferWidth = 0;
	params.BackBufferHeight = 0;
	params.BackBufferFormat = D3DFMT_UNKNOWN;
	params.BackBufferCount = 0;
	params.MultiSampleType = D3DMULTISAMPLE_NONE;
	params.MultiSampleQuality = NULL;
	params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	params.hDeviceWindow = window;
	params.Windowed = 1;
	params.EnableAutoDepthStencil = 0;
	params.AutoDepthStencilFormat = D3DFMT_UNKNOWN;
	params.Flags = NULL;
	params.FullScreen_RefreshRateInHz = 0;
	params.PresentationInterval = 0;

	// Create the device
	if (d3d9->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_NULLREF,
		window,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_DISABLE_DRIVER_MANAGEMENT,
		&params,
		&device
	) < 0) // smaller than 0 means it failed 
	{
		return false;
	}

	return true;
}

void gui::DestroyDirectX() noexcept {
	if (device) {
		device->Release();
		device = NULL;
	}
	if (d3d9) {
		d3d9->Release();
		d3d9 = NULL;
	}
}

void gui::Setup() {
	if (!SetupWindowClass("menuClass")) {
		throw std::runtime_error("Failed to create window class.");
	}

	if (!SetupWindow("Menu")) {
		throw std::runtime_error("Failed to create window");
	}

	if (!SetupDirectX()) {
		throw std::runtime_error("Failed to create directx device");
	}

	DestroyWindow();
	DestroyWindowClass();
}

void gui::SetupMenu(LPDIRECT3DDEVICE9 device) noexcept {
	// Called first time hook is run
	auto params = D3DDEVICE_CREATION_PARAMETERS{};
	device->GetCreationParameters(&params);

	// Actual window of the game
	window = params.hFocusWindow;
	originalWindowProcess = reinterpret_cast<WNDPROC>(
		SetWindowLongPtr(window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WindowProcess)) // set games window process to our window process
	);

	// Create ImGui
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(device);

	setup = true;
}

void gui::Destroy() noexcept {
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	// Restore original window process
	SetWindowLongPtr(
		window,
		GWLP_WNDPROC,
		reinterpret_cast<LONG_PTR>(originalWindowProcess)
	);

	DestroyDirectX();
}

void gui::RenderStart() noexcept {
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void gui::RenderEnd() noexcept {
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

LRESULT CALLBACK WindowProcess(
	HWND window,
	UINT message,
	WPARAM wideParam,
	LPARAM longParam
) 
{
	// Toggle
	if (GetAsyncKeyState(VK_INSERT) & 1) {
		gui::open = !gui::open;
	}

	// pass messages to imgui & dont pass to game window if gui is open
	if (gui::open && ImGui_ImplWin32_WndProcHandler(window, message, wideParam, longParam)) {
		return 1L;
	}

	// Instead of calling the game window process, call ours and then we'll forward them
	return CallWindowProc(
		gui::originalWindowProcess,
		window,
		message,
		wideParam,
		longParam
	);
}