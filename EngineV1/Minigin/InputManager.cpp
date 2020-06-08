#include "MiniginPCH.h"
#include "InputManager.h"
#include <SDL.h>
#include "imgui.h"
#include "imgui_sdl.h"


InputManager::~InputManager()
{
	for (auto key : m_pKeyboardKeys)
	{
		delete key.second;
		key.second = nullptr;
	}
}

void InputManager::Initialize()
{
	KeyboardButton* A = new KeyboardButton{"A", 113};
	m_pKeyboardKeys.insert({ A->code, A });

	std::vector< KeyboardButton* > oke;
	oke.push_back(A);
	m_KeyboardActions.insert(std::pair<std::string, std::vector< KeyboardButton* >>("OK", oke));
}

bool InputManager::ProcessInput()
{
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
		else if (e.type == SDL_KEYUP || e.type == SDL_KEYDOWN)
		{
			int key = e.key.keysym.sym & ~SDLK_SCANCODE_MASK;
			
			auto keyboardKey = m_pKeyboardKeys.find(key);
			if (keyboardKey != m_pKeyboardKeys.end())
				(*keyboardKey).second->isPressed = true;

			io.KeysDown[key] = (e.type == SDL_KEYDOWN);
			io.KeyShift = ((SDL_GetModState() & KMOD_SHIFT) != 0);
			io.KeyCtrl = ((SDL_GetModState() & KMOD_CTRL) != 0);
			io.KeyAlt = ((SDL_GetModState() & KMOD_ALT) != 0);
			io.KeySuper = ((SDL_GetModState() & KMOD_GUI) != 0);		
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

//	std::cout << m_CurrentState.Gamepad.sThumbLX << "\n";

	for (auto key : m_KeyboardActions.at("OK"))
	{
		if (key->isPressed)
			std::cout << "Yup\n";
	}
	

	return true;
}

void InputManager::DrawInterface()
{
	if (ImGui::BeginTabItem("Input"))
	{
		//ImGui::ListBox
		ImGui::EndTabItem();
	}
}


