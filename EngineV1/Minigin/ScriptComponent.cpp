#include "MiniginPCH.h"
#include "ScriptComponent.h"
#include "imgui.h"
#include "Script.h"

ScriptComponent::ScriptComponent(GameObject* pObject)
	: BaseComponent(pObject, "ScriptComponent")
{
}

ScriptComponent::~ScriptComponent()
{
	delete m_pScript;
	m_pScript = nullptr;
}

void ScriptComponent::Initialize()
{
	if (m_pScript)
		m_pScript->Initialize();
}

void ScriptComponent::LateInitialize()
{
	if (m_pScript)
		m_pScript->LateInitialize();
}

void ScriptComponent::Render()
{
}

void ScriptComponent::Update(float elapsedSec)
{
	if (m_pScript && GameInfo::GetInstance()->IsPlaying())
		m_pScript->Update(elapsedSec);
}

void ScriptComponent::DrawInterface()
{
	using namespace ImGui;
	SetNextItemOpen(true, ImGuiCond_Once);

	bool open = TreeNode(&GetName().front());
	HandleDrag();

	if (open)
	{
		Separator();
		Spacing();

		//Text if script is set or nah
		std::string text{};
		if (m_pScript)
			text = "Script set: " + m_pScript->GetName();
		else
			text = "No script set!";
		Button(text.c_str());
		
		//Set script by dragging
		if (ImGui::BeginDragDropTarget())
		{	
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Script"))
			{			
				m_pScript = GameObjectManager::GetInstance()->GetAndRemoveSelectedScript();
				m_pScript->SetGameObject(m_pGameObject);
			}

			ImGui::EndDragDropTarget();
		}

		//Draw script interface
		if (m_pScript)
			m_pScript->DrawInterface();
		TreePop();
	}

	HandleDrop();
}

void ScriptComponent::SaveAttributes(rapidxml::xml_document<>* pDoc, rapidxml::xml_node<>* pNode)
{
	if (m_pScript)
		m_pScript->SaveAttributes(pDoc, pNode);
}


void ScriptComponent::SetAttributes(Script* pScript, rapidxml::xml_node<>* pNode)
{
	pScript->SetGameObject(m_pGameObject);
	pScript->SetAttributes(pNode);
	m_pScript = pScript;
}

void ScriptComponent::SetScript(Script* pScript)
{
	if (pScript)
	{
		m_pScript = pScript;
		m_pScript->SetGameObject(m_pGameObject);
	}
}
