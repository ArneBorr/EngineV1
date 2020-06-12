#include "MiniginPCH.h"
#include "InputManager.h"
#include <SDL.h>
#include "imgui.h"
#include "imgui_sdl.h"
#include "SaveHandler.h"

InputManager::~InputManager()
{
	for (auto& key : m_pKeyboardKeys)
	{
		delete key.second;
		key.second = nullptr;
	}
	m_pKeyboardKeys.clear();
	m_KeyboardActions.clear();
	m_KeyNames.clear();
}

void InputManager::Initialize(SaveHandler* pSaveHandler)
{
	AddKey("Q", 113);
	AddKey("W", 119);
	AddKey("S", 115);
	AddKey("A", 97);
	AddKey("D", 100);
	AddKey("E", 101);
	AddKey("LEFT", 80);
	AddKey("RIGHT", 79);
	AddKey("UP", 82);
	AddKey("DOWN", 81);
	AddKey("SPACE", 32);
	AddKey("ESC", 27);
	AddKey("LSHIFT", 225);
	AddKey("LCTRL", 224);
	AddKey("LALT", 226);

	pSaveHandler->LoadInput(m_KeyboardActions, m_pKeyboardKeys);
}

bool InputManager::ProcessInput()
{
	for (auto key : m_pKeyboardKeys)
	{
		key.second->isPressed = false;
		key.second->isReleased = false;
	}


	ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
	XInputGetState(0, &m_CurrentState);

	ImGuiIO& io = ImGui::GetIO();

	int wheel = 0;

	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
		else if (e.type == SDL_MOUSEBUTTONDOWN) 
		{
		}
		else if (e.type == SDL_WINDOWEVENT)
		{
			if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
			{
				io.DisplaySize.x = static_cast<float>(e.window.data1);
				io.DisplaySize.y = static_cast<float>(e.window.data2);
			}
		}
		if (e.type == SDL_MOUSEWHEEL)
		{
			wheel = e.wheel.y;
		}
		else if (e.type == SDL_KEYDOWN )
		{
			int key = e.key.keysym.sym & ~SDLK_SCANCODE_MASK;

			auto keyboardKey = m_pKeyboardKeys.find(key);
			if (keyboardKey != m_pKeyboardKeys.end())
			{
				if (!(*keyboardKey).second->isDown)
					(*keyboardKey).second->isPressed = true;
				(*keyboardKey).second->isDown = true;
			}

			io.KeysDown[key] = true;
			io.KeyShift = ((SDL_GetModState() & KMOD_SHIFT) != 0);
			io.KeyCtrl = ((SDL_GetModState() & KMOD_CTRL) != 0);
			io.KeyAlt = ((SDL_GetModState() & KMOD_ALT) != 0);
			io.KeySuper = ((SDL_GetModState() & KMOD_GUI) != 0);		
		}
		else if (e.type == SDL_KEYUP)
		{
			int key = e.key.keysym.sym & ~SDLK_SCANCODE_MASK;

			auto keyboardKey = m_pKeyboardKeys.find(key);
			if (keyboardKey != m_pKeyboardKeys.end())
			{
				(*keyboardKey).second->isDown = false;
				(*keyboardKey).second->isReleased = true;
			}

			io.KeysDown[key] = false;
		}

		else if (e.type == SDL_TEXTINPUT)
		{
			io.AddInputCharactersUTF8(e.text.text);
		}
	}

	int mouseX, mouseY;
	const int buttons = SDL_GetMouseState(&mouseX, &mouseY);

	//Update ImGui Mouse
	io.DeltaTime = GameInfo::GetInstance()->GetElapsedSeconds();
	io.MousePos = ImVec2(static_cast<float>(mouseX), static_cast<float>(mouseY));
	io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
	io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);
	io.MouseWheel = static_cast<float>(wheel);

	//std::cout << m_CurrentState.Gamepad.sThumbLX << "\n";

	return true;
}

void InputManager::DrawInterface()
{
	using namespace ImGui;

	if (BeginTabItem("Input"))
	{
		Text("Actions");
		Separator();

		//For every Action
		for (auto action = m_KeyboardActions.begin(); action != m_KeyboardActions.end();)
		{
			bool open{ TreeNode((*action).first.c_str()) };
			PushID(&(*action).second);
			SameLine();
			if (Button("YEET"))
			{
				action = m_KeyboardActions.erase(action);
				PopID();
				continue;
			}
			if (open)
			{
				//For every Key in the action
				for (auto key = (*action).second.begin(); key != (*action).second.end();)
				{
					Text((*key)->name.c_str());
					SameLine();
					PushID((*key));
					if (Button("Remove"))
						key = (*action).second.erase(key);
					else
						++key;
					PopID();
				}

				static int currentIndex = 0;
				Combo(" ", &currentIndex, m_KeyNames);
				SameLine();

				//Add key to action
				if (Button("Add"))
				{
					auto newKey = FindKeyboardButtonByName(m_KeyNames[currentIndex]);
					if (newKey)
						(*action).second.push_back(newKey);
				}
			
				ImGui::TreePop();
			}	
			
			++action;
			Separator();
			PopID();
		}
		
		//Add action
		InputText(" ", m_ToBeAddedActionName, IM_ARRAYSIZE(m_ToBeAddedActionName));
		SameLine();
		if (Button("Add"))
		{
			m_KeyboardActions.insert({ m_ToBeAddedActionName, {} });
		}	
		EndTabItem();
	}
}

bool InputManager::IsActionPressed(const std::string& name)
{
	auto keyboardAction = m_KeyboardActions.find(name);
	if (keyboardAction != m_KeyboardActions.end())
	{
		for (auto key : (*keyboardAction).second)
		{
			if (key->isPressed)
				return true;
		}
	}
	else
		std::cout << "IsActionPressed() : Action Not Found\n";

	return false;
}

bool InputManager::IsActionDown(const std::string& name)
{
	auto keyboardAction = m_KeyboardActions.find(name);
	if (keyboardAction != m_KeyboardActions.end())
	{
		for (auto key : (*keyboardAction).second)
		{
			if (key->isDown)
				return true;
		}
	}
	else
		std::cout << "IsActionDown() : Action Not Found\n";

	
	return false;
}

bool InputManager::IsActionReleased(const std::string& name)
{
	auto keyboardAction = m_KeyboardActions.find(name);
	if (keyboardAction != m_KeyboardActions.end())
	{
		for (auto key : (*keyboardAction).second)
		{
			if (key->isReleased)
				return true;
		}
	}
	else
		std::cout << "IsActionReleased() : Action Not Found\n";

	return false;
}

void InputManager::SaveInput(SaveHandler* pSaveHandler)
{
	pSaveHandler->SaveInput(m_KeyboardActions);
}

KeyboardButton* InputManager::FindKeyboardButtonByName(const std::string& name)
{
	for (auto key : m_pKeyboardKeys)
	{
		if (key.second->name == name)
			return key.second;
	}

	std::printf("Key not found\n");
	return nullptr;
}

void InputManager::AddKey(const std::string& name, int code)
{
	KeyboardButton* key = new KeyboardButton{ name, code };
	m_pKeyboardKeys.insert({ key->id, key });
	m_KeyNames.push_back(key->name);
}


