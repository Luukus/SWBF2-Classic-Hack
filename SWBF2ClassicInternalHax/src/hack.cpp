#include "hack.h"

void hack::Init(HackContext* context) {
	// Get initial values
	context->baseAddr = (uintptr_t)GetModuleHandle("BattlefrontII.exe");
	context->game = (Game*)(context->baseAddr + 0x1A30324);

	std::cout << "Base addr: " << std::hex << context->baseAddr << std::endl;
	std::cout << "Game addr: " << std::hex << context->game << std::endl;
	std::cout << "EntityList addr: " << std::hex << context->game->entityList << std::endl;

	// Setup our functions
	functions::Setup(context, hack::FindAndSetPlayer);

	// Initialize player if injected while in game
	if (context->game->entityList) {
		hack::FindAndSetPlayer(context);
	}

	context->setup = true;
}

void hack::InitGui(HackContext* context) {
	// Create custom font config to prevent crashes with freeing font mem
	ImFontConfig* config = new ImFontConfig();
	config->FontDataOwnedByAtlas = false;

	// Load fonts
	ImGuiIO& io = ImGui::GetIO();
	headerFont = io.Fonts->AddFontFromMemoryTTF(fonts::mainFontHex, sizeof(fonts::mainFontHex), 22.f, config);
	standardFont = io.Fonts->AddFontFromMemoryTTF(fonts::mainFontHex, sizeof(fonts::mainFontHex), 18.f, config);

	// Init menu with our options
	menu->Init(menuOptionTotalTypes, {
		{ HEADER_OPTIONS, new MenuHeader("Options", headerFont, ImColor(1.f, 1.f, 1.f, 1.f)) },
		{ AIMBOT, new MenuBooleanOption("Aimbot", false) },
		{ ENEMY_ESP, new MenuBooleanOption("Enemy ESP", false) },
		{ TEAM_ESP, new MenuBooleanOption("Team ESP", false) },
		// { GOD_MODE, new MenuBooleanOption("God Mode", false) },
		{ NEW_LINE1, new MenuEmptyLine() },
		{ HEADER_SETTINGS, new MenuHeader("Settings", headerFont, ImColor(1.f, 1.f, 1.f, 1.f)) },
		{ AIMBOT_SMOOTHING, new MenuFloatOption("Aimbot Smoothing", 10.f, 0.5f) },
		{ AIMBOT_FOV, new MenuFloatOption("Aimbot FOV Radius", 20.f, 1.f) },
		{ AIMBOT_AIMAHEAD, new MenuFloatOption("Aimbot Lead Scale", 7.f, 1.f) },
	});

	context->setupGui = true;
}

void hack::Destroy(HackContext* context) {
	functions::Destroy(context);
}

// Called when the CreateCharacterList (hooked) function is called
void hack::FindAndSetPlayer(HackContext* context) {
	wchar_t* steamName = (wchar_t*)(context->baseAddr + 0x1BAC0E0);
	wchar_t* profileName = (wchar_t*)(context->baseAddr + 0x1B3A5D4);

	Character* foundChar = character::FindByName(context, steamName);
	if (!foundChar) {
		foundChar = character::FindByName(context, profileName);
	}

	if (!foundChar) {
		std::cout << "Could not find the player" << std::endl;
	}

	context->player = foundChar;
}

void hack::Update(HackContext* context, bool* isOpen, float windowWidth, float windowHeight) {
	if (!context->setup || !context->setupGui) {
		return;
	}

	Character* player = context->player;

	// God mode - works, host-only, but disabled for now
	if (character::IsAlive(player)) {
		// if (menu->GetOption(GOD_MODE)->GetBool()) {
			// player->entity->stats->health = 1000;
			// player->entity->stats->maxHealth = 1000;
		// }
	}
	
	// Update menu
	menu->Update(isOpen);
}

void hack::Render(HackContext* context, bool* isOpen, float windowWidth, float windowHeight) {
	if (!context->setup || !context->setupGui) {
		return;
	}

	// Render menu
	menu->Render(isOpen, standardFont);

	// Perform hacks if player is alive
	Character* player = context->player;

	if (character::IsAlive(player)) {
		if (menu->GetOption(TEAM_ESP)->GetBool() || menu->GetOption(ENEMY_ESP)->GetBool()) {
			hack::Esp(context, player, windowWidth, windowHeight);
		}
		if (menu->GetOption(AIMBOT)->GetBool()) {
			hack::Aimbot(context, player, windowWidth, windowHeight);
		}
	}
}

void hack::Esp(HackContext* context, Character* player, float width, float height) {
	GameCamera* camera = context->game->cameraManager->gameCamera;
	Matrix4x4 viewMatrix = camera::GetViewMatrix(camera);

	for (int i = 0; i < 64; i++) {
		// Validate entity
		Character* target = character::GetCharacter(context, i);
		if (target == player || !character::IsAlive(target)) {
			continue;
		}

		// Render ESP
		bool onSameTeam = (uintptr_t)player->team == (uintptr_t)target->team;
		if ((onSameTeam && menu->GetOption(TEAM_ESP)->GetBool())
			|| (!onSameTeam && menu->GetOption(ENEMY_ESP)->GetBool())) {
			Vec3 targetPos = { target->entity->stats->position.x, target->entity->stats->position.y, target->entity->stats->position.z };
			Vec3 playerPos = player->entity->stats->position;

			Vec3 enemyScreenPos;
			if (mathutils::WorldToScreen2(targetPos, viewMatrix, width, height, enemyScreenPos)) {
				// Get distance
				float distance = ((Vec3)(playerPos - targetPos)).SquareRoot();
				float scale = (250 - distance) / 250;

				// Get above head
				Vec3 headLoc;
				targetPos.y += 1.75;
				mathutils::WorldToScreen2(targetPos, viewMatrix, width, height, headLoc);

				// Get color based on team
				ImColor color = ImColor(0.f, 1.f, 0.f);
				if (!onSameTeam) {
					color = ImColor(1.f, 0.f, 0.f);
				}

				// Get head to feet scaling
				float sizeScale = (abs(enemyScreenPos.y - headLoc.y)) / 700.f;
				float boxScaleDiff = sizeScale * 175.f;

				if (character::IsVisible(target, player)) {
					ImGui::GetBackgroundDrawList()->AddRect({ headLoc.x - boxScaleDiff, headLoc.y }, { enemyScreenPos.x + boxScaleDiff, enemyScreenPos.y }, color, 1 - scale);
				} else {
					ImGui::GetBackgroundDrawList()->AddRectFilled({ headLoc.x - boxScaleDiff, headLoc.y }, { enemyScreenPos.x + boxScaleDiff, enemyScreenPos.y }, ImColor(color.Value.x, color.Value.y, color.Value.z, 0.5f), 1 - scale);
				}
			}
		}
	}
}

void hack::Aimbot(HackContext* context, Character* player, float width, float height) {
	GameCamera* camera = context->game->cameraManager->gameCamera;
	Matrix4x4 viewMatrix = camera::GetViewMatrix(camera);
	Vec3 playerEyePos = player->entity->eyePosition;

	float middleX = width / 2;
	float middleY = height / 2;

	float bestDist = 1000.f;
	float bestFov = 95.f;
	Vec3 bestAngle = {};
	Vec3 bestPos = {};
	Character* bestTarget = nullptr;

	// Draw FOV circle
	float maxFov = (middleX / 90) * menu->GetOption(AIMBOT_FOV)->GetFloat();
	ImGui::GetBackgroundDrawList()->AddCircle({ middleX, middleY }, maxFov, ImColor(.9f, .9f, .9f, .9f)); 

	for (int i = 0; i < 64; i++) {
		Character* target = character::GetCharacter(context, i);
		if (target == player) {
			continue;
		}

		// Check team
		if ((uintptr_t)player->team == (uintptr_t)target->team) {
			continue;
		}

		// Check alive
		if (!character::IsAlive(target)) {
			continue;
		}

		// Check if in view
		if (!character::IsVisible(target, player)) {
			continue;
		}

		Vec3 targetEyePos = target->entity->eyePosition;
		Vec3 angle = mathutils::CalcAngle(i, playerEyePos, targetEyePos);

		float dst = ((Vec3)(playerEyePos - targetEyePos)).SquareRoot();
		
		if (dst < bestDist) {
			// Check if on screen
			Vec3 targetHeadPos = { target->entity->stats->position.x, target->entity->stats->position.y + 1.35f, target->entity->stats->position.z };
			Vec3 enemyScreenPos;
			if (mathutils::WorldToScreen2(targetHeadPos, viewMatrix, width, height, enemyScreenPos)) {
				float fov = std::hypot(middleX - enemyScreenPos.x, middleY - enemyScreenPos.y);
				
				if (fov < maxFov && fov < bestFov) {
					bestDist = dst;
					bestFov = fov;
					bestAngle = angle;
					bestPos = targetHeadPos;
					bestTarget = target;
				}
			}
		}
	}

	if (bestAngle.IsZero()) {
		return;
	}

	// Account for movement in aim
	Vec3 targetVelocity = bestTarget->entity->velocity;
	if (!targetVelocity.IsZero()) {
		targetVelocity = targetVelocity.Normalize();
		targetVelocity.y = 0;
	}

	float distance = ((Vec3)(player->entity->eyePosition - bestTarget->entity->eyePosition)).SquareRoot();
	float aimAheadScale = distance / 250;
	
	float aimAhead = menu->GetOption(AIMBOT_AIMAHEAD)->GetFloat();
	Vec3 aimAheadOffset = targetVelocity * aimAheadScale * aimAhead;

	Vec3 w2s = {};
	if (mathutils::WorldToScreen2(bestPos + aimAheadOffset, viewMatrix, width, height, w2s)) {
		utils::MoveMouse2(w2s, menu->GetOption(AIMBOT_SMOOTHING)->GetFloat(), width, height);
	}

	return; // Hard return

	/**
	  The following code updates the player model to point at the target
	  It works better than the mouse movement, but it does not work non-host
	  Keeping it here for reference
	**/

	// Adjust for offset
	bestAngle.x = mathutils::WrapAngle(bestAngle.x) - 90;

	// Split the rotation into x and z
	Vec3 newRotation = bestAngle.ToRadians();
	float newX = sin(newRotation.x);
	float newZ = cos(newRotation.x);

	// Update rotation
	player->entity->stats->rotX1 = newX;
	player->entity->stats->rotX2 = newX;
	player->entity->stats->rotZ1 = newZ;
	player->entity->stats->rotZ2 = -newZ;
	player->entity->rotationY = -newRotation.y;
}