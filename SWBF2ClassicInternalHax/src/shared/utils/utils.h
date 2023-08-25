#pragma once
#include <Windows.h>
#include "../structs/vec.h"


namespace utils {

	inline void MoveMouse(Vec3 w2sTarget, float windowWidth, float windowHeight)
	{
		float centerX = windowWidth / 2.f;
		float centerY = windowHeight / 2.f;

		float enemyX = w2sTarget.x;
		float enemyY = w2sTarget.y;

		float aX = enemyX - centerX;
		float aY = enemyY - centerY;

		mouse_event(MOUSEEVENTF_MOVE, aX, aY, 0, 0);
	}

	inline void MoveMouse2(Vec3 w2sTarget, float smoothing, float windowWidth, float windowHeight) {
		float dx = w2sTarget.x - (windowWidth / 2);
		float dy = w2sTarget.y - (windowHeight / 2);

		INPUT input = { 0 };
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = MOUSEEVENTF_MOVE;
		input.mi.dx = dx / smoothing;
		input.mi.dy = dy / smoothing;
		SendInput(1, &input, sizeof(INPUT));
	}

}