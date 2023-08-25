#pragma once
#include "structs.h"
#include "shared/gui/menu/menu.h"

struct HackContext {
	// Setup
    bool showConsole = false;
    bool setup = false;
    bool setupGui = false;
	float windowWidth;
	float windowHeight;

	// Game
    uintptr_t baseAddr;
    Game* game;
    Character* player;
};