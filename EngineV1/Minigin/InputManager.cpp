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
	for (auto action : m_pKeyboardActions)
	{
		delete action;
		action = nullptr;
	}
	m_pKeyboardKeys.clear();
	m_pKeyboardActions.clear();
	m_KeyNames.clear();
}

void InputManager::Initialize(SaveHandler* pSaveHandler)
{
	//Initialize all the (needed) keys
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
	AddKey("RCTRL", 228);
	AddKey("1", 49);
	AddKey("2", 50);
	AddKey("B", 98);
	AddKey("N", 110);

	//Load the inputactions the user has made
	pSaveHandler->LoadInput(m_pKeyboardActions, m_pKeyboardKeys);
}

bool InputManager::ProcessInput()
{
	//Reset
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

			//Set smashed key to down / pressed
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
			//Set key to released and disable down
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
		for (auto actionIt = m_pKeyboardActions.begin(); actionIt != m_pKeyboardActions.end();)
		{

			KeyboardAction* pAction = (*actionIt);
			bool open{ TreeNode(pAction->name.c_str()) };
			PushID(&actionIt);

			//Delete an action
			SameLine();
			if (Button("YEET"))
			{
				actionIt = m_pKeyboardActions.erase(actionIt);
				PopID();
				continue;
			}
			if (open)
			{
				//For every Key in the action
				for (auto keyIt = pAction->keys.begin(); keyIt != pAction->keys.end();)
				{
					PushID(&(*keyIt));
					Text((*keyIt).second->name.c_str());
					SameLine();

					//Set Players that use this button for this action
					static const char* PossiblePlayerIDs[] = { "All", "0", "1"};
					int id = (int)(*keyIt).first;
					PushItemWidth(50.f);
					if (Combo(" ", &id, PossiblePlayerIDs, IM_ARRAYSIZE(PossiblePlayerIDs)))
						(*keyIt).first = (PlayerAction)id;

					//Remove key
					else if (Button("Remove"))
					{
						keyIt = pAction->keys.erase(keyIt);
					}
					else
						++keyIt;


					PopID();
				}

				static int currentIndex = 0;
				Combo(" ", &currentIndex, m_KeyNames);
				SameLine();

				PushID(&(*actionIt)->keys);

				//Add key to action
				if (Button("Add"))
				{
					auto pNewKey = FindKeyboardButtonByName(m_KeyNames[currentIndex]);
					if (pNewKey)
						pAction->keys.push_back({ PlayerAction::All, pNewKey });
				}
				
				PopID();
				TreePop();
			}	
			
			++actionIt;
			Separator();
			PopID();
		}
		
		//Add action
		InputText(" ", m_ToBeAddedActionName, IM_ARRAYSIZE(m_ToBeAddedActionName));
		SameLine();
		if (Button("Add"))
		{
			m_pKeyboardActions.push_back( new KeyboardAction{ m_ToBeAddedActionName });
		}	
		EndTabItem();
	}
}

bool InputManager::IsActionPressed(const std::string& name, PlayerAction playerID)
{
	auto keyboardActionIt = std::find_if(m_pKeyboardActions.begin(), m_pKeyboardActions.end(), [name](KeyboardAction* pAction) { return name == pAction->name; });
	if (keyboardActionIt != m_pKeyboardActions.end())
	{
		for (auto pKey : (*keyboardActionIt)->keys)
		{
			if (pKey.second->isPressed && (playerID == PlayerAction::All || pKey.first == PlayerAction::All || (pKey.first == playerID)))
					return true;
		}
	}
	else
		std::cout << "IsActionPressed() : Action Not Found\n";

	return false;
}

bool InputManager::IsActionDown(const std::string& name, PlayerAction playerID)
{
	auto keyboardActionIt = std::find_if(m_pKeyboardActions.begin(), m_pKeyboardActions.end(), [name](KeyboardAction* pAction) { return name == pAction->name; });
	if (keyboardActionIt != m_pKeyboardActions.end())
	{
		for (auto pKey : (*keyboardActionIt)->keys)
		{
			if (pKey.second->isDown && (playerID == PlayerAction::All || pKey.first == PlayerAction::All || (pKey.first == playerID)))
				return true;
		}
	}
	else
		std::cout << "IsActionDown() : Action Not Found\n";

	
	return false;
}

bool InputManager::IsActionReleased(const std::string& name, PlayerAction playerID)
{
	auto keyboardActionIt = std::find_if(m_pKeyboardActions.begin(), m_pKeyboardActions.end(), [name](KeyboardAction* pAction) { return name == pAction->name; });
	if (keyboardActionIt != m_pKeyboardActions.end())
	{
		for (auto pKEy : (*keyboardActionIt)->keys)
		{
			if (pKEy.second->isReleased && (playerID == PlayerAction::All || pKEy.first == PlayerAction::All || (pKEy.first == playerID)))
				return true;
		}
	}
	else
		std::cout << "IsActionReleased() : Action Not Found\n";

	return false;
}

void InputManager::SaveInput(SaveHandler* pSaveHandler)
{
	pSaveHandler->SaveInput(m_pKeyboardActions);
}

KeyboardButton* InputManager::FindKeyboardButtonByName(const std::string& name)
{
	for (auto pKey : m_pKeyboardKeys)
	{
		if (pKey.second->name == name)
			return pKey.second;
	}

	std::printf("Key not found\n");
	return nullptr;
}

void InputManager::AddKey(const std::string& name, int code)
{
	KeyboardButton* pKey = new KeyboardButton{ name, code };
	m_pKeyboardKeys.insert({ pKey->id, pKey });
	m_KeyNames.push_back(pKey->name);
}


