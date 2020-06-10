#include "MiniginPCH.h"
#include "Sprite.h"
#include "imgui.h"
#include "TextureComponent.h"

Sprite::Sprite(GameObject* pObject,const std::string& name)
	: m_pTexture{ new TextureComponent(pObject, m_TexturePath) }
{
	strcpy_s(m_Name, name.c_str());
}

Sprite::~Sprite()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}

Sprite::Sprite(const Sprite& other)
{
	m_pTexture = new TextureComponent{ *other.m_pTexture };
	m_SrcRect = other.m_SrcRect;
	strncpy(m_Name, other.m_Name, IM_ARRAYSIZE(other.m_Name));
	strncpy(m_TransitionName, other.m_TransitionName, IM_ARRAYSIZE(other.m_TransitionName));
	strncpy(m_TexturePath, other.m_TexturePath, IM_ARRAYSIZE(other.m_TexturePath));
	m_TimeBetweenFrames = other.m_TimeBetweenFrames;
	m_SpacePerFrame = other.m_SpacePerFrame;
	m_Timer = 0;
	m_Rows = other.m_Rows;
	m_Columns = other.m_Columns;
	m_CurrentFrame = other.m_CurrentFrame;
	m_TotalFrames = other.m_TotalFrames;
}

void Sprite::Update(float elapsedSec)
{
	UNREFERENCED_PARAMETER(elapsedSec);

	m_Timer += elapsedSec;
	if (m_Timer > m_TimeBetweenFrames)
	{
		++m_CurrentFrame;
		m_CurrentFrame = m_CurrentFrame % m_TotalFrames;

		m_SrcRect.x = (m_SrcRect.z + m_SpacePerFrame) * (m_CurrentFrame % m_Columns);
		m_SrcRect.y = m_SrcRect.w * (m_CurrentFrame % m_Rows);

		m_Timer = 0;
	}
}

void Sprite::Render() const
{
	m_pTexture->SetSourceRect(m_SrcRect);
	m_pTexture->Render();
}

void Sprite::SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node)
{
	node->append_attribute(doc->allocate_attribute("Name", m_Name));
	m_pTexture->SaveAttributes(doc, node);
	node->append_attribute(doc->allocate_attribute("SpriteWidth", FloatToXMLChar(doc, m_SrcRect.z)));
	node->append_attribute(doc->allocate_attribute("SpriteHeight", FloatToXMLChar(doc, m_SrcRect.w)));
	node->append_attribute(doc->allocate_attribute("TransitionName", m_TransitionName));
	node->append_attribute(doc->allocate_attribute("TexturePath", m_TexturePath));
	node->append_attribute(doc->allocate_attribute("TimeBetweenFrames", FloatToXMLChar(doc, m_TimeBetweenFrames)));
	node->append_attribute(doc->allocate_attribute("SpacePerFrame", FloatToXMLChar(doc, m_SpacePerFrame)));
	node->append_attribute(doc->allocate_attribute("Rows", IntToXMLChar(doc, m_Rows)));
	node->append_attribute(doc->allocate_attribute("Columns", IntToXMLChar(doc, m_Columns)));
}

void Sprite::SetAttributes(TextureComponent* pTexture, const std::string& transitionName, const std::string& texturePath, float spriteWidth, float spriteHeight, float timeBetweenFrames, float spacePerFrame, int rows, int columns)
{
	delete m_pTexture;
	m_pTexture = nullptr;
	m_pTexture = pTexture;
	strcpy_s(m_TransitionName, transitionName.c_str());
	strcpy_s(m_TexturePath, texturePath.c_str());
	m_SrcRect.z = spriteWidth;
	m_SrcRect.w = spriteHeight;
	m_TimeBetweenFrames = timeBetweenFrames;
	m_SpacePerFrame = spacePerFrame;
	m_Rows = rows;
	m_Columns = columns;
}

void Sprite::DrawInterface()
{
	using namespace ImGui;

	if (TreeNode(m_Name))
	{
		InputText("Name", m_Name, IM_ARRAYSIZE(m_Name));
		if (InputText("Texture Path", m_TexturePath, IM_ARRAYSIZE(m_TexturePath)))
			m_pTexture->SetTexture(m_TexturePath);
		InputText("Transition Bool", m_TransitionName, IM_ARRAYSIZE(m_TransitionName));
		InputFloat("Time between frames", &m_TimeBetweenFrames);
		InputFloat("Width per Frame", &m_SrcRect.z);
		InputFloat("Height per Frame", &m_SrcRect.w);
		InputFloat("Space Between Frames", &m_SpacePerFrame);
		if (InputInt("Rows", &m_Rows))
			m_TotalFrames = m_Rows * m_Columns;
		if (InputInt("Columns", &m_Columns))
			m_TotalFrames = m_Rows * m_Columns;
		TreePop();
	}
}
