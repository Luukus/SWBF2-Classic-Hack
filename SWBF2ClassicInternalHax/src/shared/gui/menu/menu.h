#pragma once
#include <map>
#include <Windows.h>
#include <iostream>

#include "../external/ImGui/imgui.h"
#include "menuOptions.h"

template <typename Type>
class Menu
{
private:
	std::map<Type, MenuOption*> menuOptions;
	int menuIndex;
	int menuSize;

public:
	void Init(int typeSize, std::map<Type, MenuOption*> options) {
		menuSize = typeSize;
		menuOptions = options;
		menuIndex = 1;
	}

	void Update(bool* isOpen) {
		if (*isOpen) {
			if (GetAsyncKeyState(VK_UP) & 1) {
				GoUp();
			}
			if (GetAsyncKeyState(VK_DOWN) & 1) {
				GoDown();
			}
			if (GetAsyncKeyState(VK_RIGHT) & 1) {
				menuOptions[(Type)menuIndex]->OnIncrease();
			}
			if (GetAsyncKeyState(VK_LEFT) & 1) {
				menuOptions[(Type)menuIndex]->OnDecrease();
			}
		}
	}

	void Render(bool* isOpen, ImFont* standardFont) {
		if (*isOpen) {
			ImGui::PushFont(standardFont);

			ImGui::Begin("Menu (Press Insert)", isOpen);
			ImGui::SetWindowSize({ 0, 0 });

			// Render options
			for (auto const& [index, option] : menuOptions) {
				option->Render(menuIndex, index);
			}

			ImGui::NewLine();
			ImGui::TextColored(ImColor(0.75f, 0.75f, 0.75f, 0.75f), "Use arrow keys to navigate");
			ImGui::TextColored(ImColor(0.95f, 0.95f, 0.95f, 0.95f), "Created by github.com/luukus");

			ImGui::End();

			ImGui::PopFont();
		}
	}

	MenuOption* GetOption(Type option) {
		return menuOptions[option];
	}

	void GoUp() {
		menuIndex -= 1;
		if (menuIndex < 0) {
			GoDown();
			return;
		}

		if (!menuOptions.contains((Type)menuIndex) || !menuOptions[(Type)menuIndex]->canSelect) {
			GoUp();
		}
	}

	void GoDown() {
		menuIndex += 1;
		while (menuIndex <= menuSize && !menuOptions.contains((Type)menuIndex)) {
			menuIndex += 1;
		}

		if (menuIndex >= menuSize) {
			GoUp();
			return;
		}

		if (!menuOptions.contains((Type)menuIndex) || !menuOptions[(Type)menuIndex]->canSelect) {
			GoDown();
		}
	}
};

