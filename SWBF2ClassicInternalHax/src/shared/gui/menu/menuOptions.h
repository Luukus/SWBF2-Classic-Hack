#pragma once
#include <string>
#include <format>

#include "../external/ImGui/imgui.h"

struct MenuOption {

	static inline ImColor SELECTED_COLOR = ImColor(1.f, .95f, .05f, 1.f);

	std::string name;
	bool canSelect;

	MenuOption(std::string _name, bool _canSelect) : name(_name), canSelect(_canSelect) {}
	
	virtual void OnIncrease() {};
	virtual void OnDecrease() {};

	virtual void Render(int selectedIndex, int thisIndex) = 0;
	
	virtual bool GetBool() { return false; };
	virtual float GetFloat() { return 0.f; };
};

struct MenuHeader : MenuOption {

	ImFont* font = nullptr;
	ImColor color = {};

	MenuHeader(std::string _name, ImFont* _font, ImColor _color) :
		MenuOption(_name, false), font(_font), color(_color) {}

	void Render(int selectedIndex, int thisIndex) override {
		ImGui::PushFont(font);
		ImGui::TextColored(ImColor(1.f, 1.f, 1.f, 1.f), name.c_str());
		ImGui::PopFont();
	}
};

struct MenuEmptyLine : MenuOption {

	MenuEmptyLine() : MenuOption("", false) {}

	void Render(int selectedIndex, int thisIndex) override {
		ImGui::NewLine();
	}
};

struct MenuBooleanOption : MenuOption {

	bool enabled = false;

	MenuBooleanOption(std::string _name, bool initEnabled) :
		MenuOption(_name, true), enabled(initEnabled) {}

	void OnIncrease() override {
		enabled = true;
	}

	void OnDecrease() override {
		enabled = false;
	}

	bool GetBool() override {
		return enabled;
	}

	void Render(int selectedIndex, int thisIndex) override {
		ImVec4 enabledColor = { enabled ? 0.f : 1.f, enabled ? 1.f : 0.f, 0.f, 1.f };
		ImVec4 optionColor = ImColor(.95f, .95f, .95f, 1.f);
		if (selectedIndex == thisIndex) {
			optionColor = SELECTED_COLOR;
		}

		ImGui::TextColored(optionColor, name.c_str());
		ImGui::SameLine();
		ImGui::TextColored(enabledColor, enabled ? "Enabled" : "Disabled");
	}
};

struct MenuFloatOption : MenuOption {

	float value = 0.f;
	float valueChange = 1.f;

	MenuFloatOption(std::string _name, float initValue, float valChange) :
		MenuOption(_name, true), value(initValue), valueChange(valChange) {}

	void OnIncrease() override {
		value += valueChange;
	}

	void OnDecrease() override {
		value -= valueChange;
	}

	void Render(int selectedIndex, int thisIndex) override {
		ImVec4 color = { .95f, .95f, .95f, 1.f };
		ImVec4 optionColor = color;
		if (selectedIndex == thisIndex) {
			optionColor = SELECTED_COLOR;
		}

		ImGui::TextColored(optionColor, name.c_str());
		ImGui::SameLine();
		ImGui::TextColored(color, "%.2f", value);
	}

	float GetFloat() override {
		return value;
	}
};

