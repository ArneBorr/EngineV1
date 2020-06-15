#pragma once
class ImGuiWindows
{
public:
	ImGuiWindows();

	void Render();
	const Vector4f& GetEditorDimensions() { return m_EditorDimensions; }

private:
	static const int m_NrOfWindows = 5;

	Vector4f m_EditorDimensions; //x,y = left bottom | z,w = width/height
	Vector2f m_WindowPositions[m_NrOfWindows] = {};
	Vector2f m_WindowSizes[m_NrOfWindows] = {};

	void StartWindow(int index);
	void EndWindow();
	void EditorWindow();
};

