#pragma once

#include "GameObject.h"

class BaseComponent
{
public:
	BaseComponent(GameObject* pGameObject, const std::string& name);
	virtual ~BaseComponent() = default;

	BaseComponent(const BaseComponent& other) noexcept;
	BaseComponent(BaseComponent&& other) = delete;
	BaseComponent& operator=(const BaseComponent& other) = delete;
	BaseComponent& operator=(BaseComponent&& other) = delete;

	virtual void Initialize() {};
	virtual void Render() = 0;
	virtual void Update(float elapsedSec) = 0;

	virtual void DrawInterface() = 0;

	virtual void AdaptToFullscreen(const Vector2f& ratio) { UNREFERENCED_PARAMETER(ratio); }

	const std::string& GetName() const { return m_Name; }
	void SetName(const std::string& name) { m_Name = name; }

	const unsigned int& GetIndexHierarchy() const { return m_IndexInHierarchy; }; // const & since ImGui needs an lvalue
	void SetIndexInHierarchy(unsigned int index) { m_IndexInHierarchy = index; };

	virtual void SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node) = 0;

	virtual void SetObject(GameObject* pObject) { m_pGameObject = pObject; }

protected:
	GameObject* m_pGameObject;
	std::string m_Name;
	unsigned int m_IndexInHierarchy{};

	void HandleDrag();
	void HandleDrop();
};

