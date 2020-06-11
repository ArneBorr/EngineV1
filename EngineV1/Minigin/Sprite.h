#pragma once

class GameObject;
class TextureComponent;

class Sprite
{
public:
	//Sprite() {};
	Sprite(GameObject* pObject, const std::string& name);
	~Sprite();

	Sprite(const Sprite& other) = delete;
	Sprite(Sprite&& other) = delete;
	Sprite& operator=(const Sprite& other) = delete;
	Sprite& operator=(Sprite&& other) = delete;

	void Update(float elapsedSec);
	void Render() const;

	void SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node);
	void SetAttributes(TextureComponent* pTexture, const std::string& texturePath, float spriteWidth, float spriteHeight, float timeBetweenFrames, 
		float spacePerFrame, int rows, int columns);
	void DrawInterface();

	std::string GetName() { return m_Name;  }
	char* GetNameRef() { return m_Name;  }

private:
	TextureComponent* m_pTexture{ nullptr };
	Vector4f m_SrcRect{0, 0, 16, 16};
	char m_Name[25]{ };
	char m_TexturePath[40]{ };
	float m_TimeBetweenFrames{ 1 };
	float m_SpacePerFrame{ 16 };
	float m_Timer{ 0 };
	int m_Rows{ 1 };
	int m_Columns{ 8 };
	int m_CurrentFrame{ 0 };
	int m_TotalFrames{ 8 };
};

