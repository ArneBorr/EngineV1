#pragma once
#include <XInput.h>
#include "Singleton.h"
#include <map>



struct KeyboardButton
{
	KeyboardButton() {};
	KeyboardButton(const std::string& _name, int _code) : name{ _name }, id{ _code } { }
	std::string name{ " " };
	int id = 0;
	bool isPressed = false;
	bool isDown = false;
	bool isReleased = false;
};

enum class PlayerAction : int
{
	All = 0,
	One = 1,
	Two = 2
};

struct KeyboardAction
{
	KeyboardAction(const std::string& id) : name(id) {}
	//No destructor, keys are not owned by le actione

	std::vector<std::pair<PlayerAction, KeyboardButton*>> keys{};
	std::string name{};
	PlayerAction players{ PlayerAction::All };
};


class SaveHandler;

class InputManager final : public Singleton<InputManager>
{
public:
	~InputManager();
	void Initialize(SaveHandler* pSaveHandler);
	bool ProcessInput();
	void DrawInterface();
	bool IsActionPressed(const std::string& name, PlayerAction playerID);
	bool IsActionDown(const std::string& name, PlayerAction playerID);
	bool IsActionReleased(const std::string& name, PlayerAction playerID);
	void SaveInput(SaveHandler* pSaveHandler);

private:
	std::map<int, KeyboardButton*> m_pKeyboardKeys{};
	std::vector<KeyboardAction*> m_pKeyboardActions{};
	std::vector<std::string> m_KeyNames; // Needed for imgui :( Fix if enough time, but yeah hehe
	XINPUT_STATE m_CurrentState{};
	char m_ToBeAddedActionName[20]{ "" };

	KeyboardButton* FindKeyboardButtonByName(const std::string& name);
	void AddKey(const std::string& name, int code);
};


