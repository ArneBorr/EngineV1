#pragma once

#include "Singleton.h"
#include <chrono>
#include "structs.h"


class GameInfo final : public Singleton<GameInfo>
{
public:
	void Start();
	void Update();

	void DrawInterface();
	
	static const Vector2f& GetWindowSize() { return m_WindowSize; };
	float GetElapsedSeconds();

	bool IsPlaying() const { return m_IsPlaying; }
	bool IsFullscreen() const { return m_IsFullscreen; }


private:
	static const Vector2f m_WindowSize;

	std::chrono::time_point<std::chrono::steady_clock> m_CurrFrame;
	std::chrono::time_point<std::chrono::steady_clock> m_PrevFrame;

	float m_ElapsedTime{};

	bool m_IsPlaying = false;
	bool m_IsFullscreen = false;
};

