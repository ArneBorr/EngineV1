#include "MiniginPCH.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "BaseComponent.h"
#include "Components.h"
#include "imgui.h"

GameObject::GameObject(const std::string& name)
	: m_Name{ name }
{
}

GameObject::~GameObject()
{
	for (unsigned int i{}; i < m_pComponents.size(); i++)
	{
		delete m_pComponents[i];
		m_pComponents[i] = nullptr;
	}
}

void GameObject::Update(float elapsedSec)
{
	for (auto pComp : m_pComponents)
	{
		pComp->Update(elapsedSec);
	}
}

void GameObject::Render() const
{
	for (auto pComp : m_pComponents)
	{
		pComp->Render();
	}
}

void GameObject::DrawInterfaceScene()
{
	bool open = ImGui::TreeNode(&m_Name.front());

	//if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
//{
//	ImGui::SetDragDropPayload("DND_DEMO_CELL", &n, sizeof(int));    // Set payload to carry the index of our item (could be anything)
//	if (mode == Mode_Copy) { ImGui::Text("Copy %s", names[n]); }    // Display preview (could be anything, e.g. when dragging an image we could decide to display the filename and a small preview of the image, etc.)
//	if (mode == Mode_Move) { ImGui::Text("Move %s", names[n]); }
//	if (mode == Mode_Swap) { ImGui::Text("Swap %s", names[n]); }
//	ImGui::EndDragDropSource();
//}
//if (ImGui::BeginDragDropTarget())
//{
//	if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_DEMO_CELL"))
//	{
//		IM_ASSERT(payload->DataSize == sizeof(int));
//		int payload_n = *(const int*)payload->Data;
//		if (mode == Mode_Copy)
//		{
//			names[n] = names[payload_n];
//		}
//		if (mode == Mode_Move)
//		{
//			names[n] = names[payload_n];
//			names[payload_n] = "";
//		}
//		if (mode == Mode_Swap)
//		{
//			const char* tmp = names[n];
//			names[n] = names[payload_n];
//			names[payload_n] = tmp;
//		}
//	}
//	ImGui::EndDragDropTarget();

	if (open)
	{
		
		ImGui::TreePop();
	}

	if (ImGui::IsItemClicked())
		GameObjectManager::GetInstance()->SetSelectedGameObject(this);

}

void GameObject::DrawInterfaceComponents()
{
	ImGui::Text("Text");
	ImGui::InputText("Text", &m_Name.front(), 128);
	//List of components on gameobject
	for (unsigned int i{}; i < m_pComponents.size(); i++)
	{
		m_pComponents[i]->DrawInterface();
	}

	//List of components that you can add
	const char* PossibleComponents[] = { "TransformComponent", "TextureComponent", "TextComponent" };

	static int currentAddableCompIndex = 0;
	ImGui::Separator();
	ImGui::Combo(" ", &currentAddableCompIndex, PossibleComponents, IM_ARRAYSIZE(PossibleComponents));
	ImGui::SameLine();

	//if clicked, add component that is selected
	if (ImGui::Button("Add"))
	{
		BaseComponent* pComponent = nullptr;
		std::string item = PossibleComponents[currentAddableCompIndex];

		if (item == "TransformComponent")
		{
			pComponent = new TransformComponent(this);
		}
		else if (item == "TextureComponent")
		{
			pComponent = new TextureComponent(this, "background.jpg");
		}
		else if (item == "TextComponent")
		{
			pComponent = new TextComponent(this);
		}

		if (pComponent)
			AddComponent(pComponent);
	}
}

void GameObject::AddComponent(BaseComponent* pComponent)
{
	//Prevent every component having the same name
	std::string name = pComponent->GetName();
	name += std::to_string(m_pComponents.size());
	pComponent->SetName(name);

	m_pComponents.push_back(pComponent);
}

