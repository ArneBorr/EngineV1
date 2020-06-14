#pragma once

#include "Singleton.h"
#include <chrono>
#include "structs.h"

class SaveHandler;

class GameInfo final : public Singleton<GameInfo>
{
public:
	~GameInfo();
	void Initialize(SaveHandler* pSaveHandler);
	void StartTimer();
	void Update();

	void DrawInterface();

	void SetWindowSize(const Vector2f& size) { m_WindowSize = size; }
	

	const Vector2f& GetWindowSize() { return m_WindowSize; };
	float GetElapsedSeconds();

	bool IsPlaying() const { return m_IsPlaying; }
	bool IsFullscreen() const { return m_IsFullscreen; }

	SaveHandler* GetSaveHandler() { return m_pSaveHandler; }
private:
	
	Vector2f m_WindowSize;

	SaveHandler* m_pSaveHandler{ nullptr };

	std::chrono::time_point<std::chrono::steady_clock> m_CurrFrame;
	std::chrono::time_point<std::chrono::steady_clock> m_PrevFrame;

	float m_ElapsedTime{};

	bool m_IsPlaying = false;
	bool m_IsFullscreen = false;
};

