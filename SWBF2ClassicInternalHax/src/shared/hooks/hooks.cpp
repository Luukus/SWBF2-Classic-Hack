#include "hooks.h"

// Due to templating, we must define this within the header file
void hooks::Setup(HackContext* context, void (*userInitGui)(HackContext*), void (*userRender)(HackContext*, bool*, float, float)) {
	// Store user render function
	userContext = context;
	userInitGuiFunc = userInitGui;
	userRenderFunc = userRender;

	if (MH_Initialize()) {
		throw std::runtime_error("Unable to initialize minhook");
	}

	if (MH_CreateHook(
		VirtualFunction(gui::device, 42), // getting the 42nd function
		&EndScene,
		reinterpret_cast<void**>(&EndSceneOriginal)
	)) {
		throw std::runtime_error("Unable to hook EndScene()");
	}

	if (MH_CreateHook(
		VirtualFunction(gui::device, 16), // getting the 16th function
		&Reset,
		reinterpret_cast<void**>(&ResetOriginal)
	)) {
		throw std::runtime_error("Unable to hook Reset()");
	}

	gui::DestroyDirectX();
}

void hooks::EnableHooks() {
	if (MH_EnableHook(MH_ALL_HOOKS)) {
		throw std::runtime_error("Unable to enable hooks");
	}
}

HRESULT __stdcall hooks::Reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params) noexcept {
	ImGui_ImplDX9_InvalidateDeviceObjects();
	const auto result = ResetOriginal(device, device, params);
	ImGui_ImplDX9_CreateDeviceObjects();

	return result;
}

long __stdcall hooks::EndScene(IDirect3DDevice9* device) noexcept {
	static const auto returnAddress = _ReturnAddress();
	const auto result = EndSceneOriginal(device, device);

	// Prevent double rendering
	if (_ReturnAddress() == returnAddress) {
		return result;
	}

	if (!gui::setup) {
		gui::SetupMenu(device);
		userInitGuiFunc(userContext);
	}

	// Get viewport
	D3DVIEWPORT9 viewport;
	device->GetViewport(&viewport);

	// Update dimensions
	windowWidth = viewport.Width;
	windowHeight = viewport.Height;
	userContext->windowWidth = windowWidth;
	userContext->windowHeight = windowHeight;

	gui::RenderStart();
	try {
		if (!renderError) {
			userRenderFunc(userContext, &gui::open, windowWidth, windowHeight);
		}
	}
	catch (const std::exception& error) {
		MessageBeep(MB_ICONERROR);
		MessageBox(0, error.what(), "Render Error", MB_OK | MB_ICONEXCLAMATION);
		renderError = true;
	}
	gui::RenderEnd();

	return result;
}

void hooks::Destroy() noexcept {
	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);
	MH_Uninitialize();
}