#pragma once

#include <stdexcept>
#include <intrin.h>

#include "../external/minhook/minhook.h"

#include "../external/ImGui/imconfig.h"
#include "../external/ImGui/imgui_impl_win32.h"
#include "../external/ImGui/imgui_impl_dx9.h"

#include "../gui/gui.h"
#include "../../context.h"

namespace hooks {

	// User functions and variables
	inline HackContext* userContext;
	inline void (*userRenderFunc)(HackContext*, bool*, float, float);
	inline void (*userInitGuiFunc)(HackContext*);
	inline float windowWidth;
	inline float windowHeight;
	inline bool renderError = false;

	constexpr void* VirtualFunction(void* thisptr, size_t index) noexcept {
		return (*static_cast<void***>(thisptr))[index]; // returns an array of pointers and gets the pointer at index
	}

	// EndScene Hook
	using EndSceneFn = long(__thiscall*)(void*, IDirect3DDevice9*) noexcept;
	inline EndSceneFn EndSceneOriginal = nullptr;
	long __stdcall EndScene(IDirect3DDevice9* device) noexcept;

	// Reset hook - handles resizing and things
	using ResetFn = HRESULT(__thiscall*)(void*, IDirect3DDevice9*, D3DPRESENT_PARAMETERS*) noexcept;
	inline ResetFn ResetOriginal = nullptr;
	HRESULT __stdcall Reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params) noexcept;

	void Setup(HackContext* context, void (*InitGui)(HackContext*), void (*userRender)(HackContext*, bool*, float, float));

	void EnableHooks();

	void Destroy() noexcept;

};

