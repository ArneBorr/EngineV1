#pragma once
#include <XInput.h>
#include "Singleton.h"
#include <map>


struct KeyboardButton
{
	KeyboardButton() {};
	KeyboardButton(const std::string& _name, int _code) : name{ _name }, code{ _code } { }
	std::string name{ " " };
	int code = 0;
	bool isPressed = false;
	bool isDown = false;
	bool isReleased = false;
	bool wasPressedLastFrame = false;
};

class InputManager final : public Singleton<InputManager>
{
public:
	~InputManager();
	void Initialize();
	bool ProcessInput();
	void DrawInterface();
	bool IsActionPressed(const std::string& name);
	bool IsActionDown(const std::string& name);
	bool IsActionReleased(const std::string& name);

private:
	std::map<int, KeyboardButton*> m_pKeyboardKeys;
	std::map<std::string, std::vector<KeyboardButton*>> m_KeyboardActions;
	std::vector<std::string> m_KeyNames; // Needed for imgui :( Fix if enough time, but yeah hehe
	XINPUT_STATE m_CurrentState{};

	KeyboardButton* FindKeyboardButtonByName(const std::string& name);
	void AddKey(const std::string& name, int code);
};


