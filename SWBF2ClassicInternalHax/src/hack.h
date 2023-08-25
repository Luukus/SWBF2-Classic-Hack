#pragma once
#include <string>
#include <iostream>
#include <algorithm>
#include <Windows.h>
#include <map>

#include "../external/ImGui/imgui.h"

#include "shared/gui/menu/menu.h"
#include "shared/structs/vec.h"
#include "shared/structs/matrix.h"
#include "shared/utils/mathUtils.h"
#include "shared/utils/utils.h"

#include "context.h"
#include "functions.h"
#include "character.h"
#include "structs.h"
#include "camera.h"
#include "fonts.h"

inline int menuOptionTotalTypes = 10;
enum MenuOptionTypes {
	HEADER_OPTIONS,
	AIMBOT,
	ENEMY_ESP,
	TEAM_ESP,
	GOD_MODE,
	NEW_LINE1,
	HEADER_SETTINGS,
	AIMBOT_SMOOTHING,
	AIMBOT_FOV,
	AIMBOT_AIMAHEAD
};

namespace hack {

	inline Menu<MenuOptionTypes>* menu = new Menu<MenuOptionTypes>();
	inline ImFont* headerFont = nullptr;
	inline ImFont* standardFont = nullptr;

	void Init(HackContext* context);
	void InitGui(HackContext* context); // called after ImGui init
	void Update(HackContext* context, bool* isOpen, float windowWidth, float windowHeight);
	void Render(HackContext* context, bool* isOpen, float windowWidth, float windowHeight);
	void Destroy(HackContext* context);

	void FindAndSetPlayer(HackContext* context);
	void Esp(HackContext* context, Character* player, float width, float height);
	void Aimbot(HackContext* context, Character* player, float width, float height);
};

