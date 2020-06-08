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
};

class InputEvents final
{
	InputEvents();

	//std::map<int, >
};

class InputManager final : public Singleton<InputManager>
{
public:
	~InputManager();
	void Initialize();
	bool ProcessInput();
	void DrawInterface();


private:
	std::map<int, KeyboardButton*> m_pKeyboardKeys;
	std::map<std::string, std::vector<KeyboardButton*>> m_KeyboardActions;
	XINPUT_STATE m_CurrentState{};
};


