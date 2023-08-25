#pragma once
#include "structs.h"
#include "functions.h"

constexpr auto ENTITY_TRACELINE_ENT_OFFSET = 0x240;

namespace character {
	
	inline Character* GetCharacter(HackContext* context, int index) {
		uintptr_t baseAddr = (uintptr_t)context->game->entityList[0];
		uintptr_t add = (0x01B0 * index);
		return (Character*)(baseAddr + add); // sizeof(Character) is not working correctly, unsure why
	}

	inline Character* FindByName(HackContext* context, wchar_t* name) {
		if (wcscmp(name, L"") == 0) {
			return nullptr;
		}

		for (int i = 0; i < 64; i++) {
			Character* character = character::GetCharacter(context, i);
			if (wcscmp(character->nameStart, name) == 0) {
				return character;
			}
		}
		return nullptr;
	}

	inline bool IsAlive(Character* character) {
		return character 
			&& character->entity 
			&& character->entity->stats 
			&& character->entity->isInAir >= 0 && character->entity->isInAir <= 10;
	}

	inline bool IsVisible(Character* target, Character* player) {
		// return 1; // Enable if in debug mode

		// Build entityArray
		const uintptr_t entityArray[2] = {
			(uintptr_t)((uintptr_t)player->entity - ENTITY_TRACELINE_ENT_OFFSET),
			(uintptr_t)((uintptr_t)target->entity - ENTITY_TRACELINE_ENT_OFFSET)
		};
		float* enemyPosPtr = (float*)((Vec3*)&player->entity->eyePosition);
		float* playerPosPtr = (float*)((Vec3*)&target->entity->eyePosition);
		int entityArrayPtr = (int)entityArray;

		// FYI: This call will fail if the DLL is built in Debug mode
		return !functions::TraceLine(enemyPosPtr, playerPosPtr, entityArrayPtr, 2, 144);
	}

}