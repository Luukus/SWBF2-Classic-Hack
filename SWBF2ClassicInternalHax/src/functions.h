#pragma once

#include "windows.h"
#include <emmintrin.h>
#include <fstream>
#include <iomanip>

#pragma comment(lib,"ws2_32.lib")	

#include "../external/minhook/minhook.h"

#include "context.h"

namespace functions {

	inline HackContext* context;
	inline void (*AfterCreateCharacterList)(HackContext*);
	inline std::ofstream sendLog;
	inline std::ofstream recvLog;

	// Traceline
	typedef bool(__fastcall * _TraceLine)(float*, float*, int, int, int);
	inline _TraceLine TraceLine = nullptr;

	// CreateCharacterList hook
	using CreateCharacterListFn = int(__thiscall*)(DWORD*) noexcept;
	inline CreateCharacterListFn CreateCharacterListOriginal = nullptr;
	inline int __fastcall CreateCharacterList(DWORD* thisInstance) noexcept {
		const auto result = CreateCharacterListOriginal(thisInstance);
		AfterCreateCharacterList(context);
		return result;
	}

	inline void Setup(HackContext* ctx, void (*AfterCreateCharacterListFunc)(HackContext*)) {
		context = ctx;
		AfterCreateCharacterList = AfterCreateCharacterListFunc;

		sendLog.open("send.txt", std::ios::app);

		// Assign functions to call
		TraceLine = (_TraceLine)(context->baseAddr + 0x5E400);

		// Hook CreateCharacterList
		if (MH_CreateHook(
			reinterpret_cast<LPVOID>(ctx->baseAddr + 0x24E510),
			&CreateCharacterList,
			reinterpret_cast<void**>(&CreateCharacterListOriginal)
		)) {
			throw std::runtime_error("Unable to hook CreateCharacterList()");
		}
	}

	inline void Destroy(HackContext* context) {
		sendLog.close();
	}

}