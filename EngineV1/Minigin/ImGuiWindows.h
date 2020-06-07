#pragma once
class ImGuiWindows
{
public:
	ImGuiWindows();

	void Render();

private:
	static const int m_NrOfWindows = 4;

	Vector2f m_WindowPositions[m_NrOfWindows]{};
	Vector2f m_WindowSizes[m_NrOfWindows]{};

	void StartWindow(int index);
	void EndWindow();
};

