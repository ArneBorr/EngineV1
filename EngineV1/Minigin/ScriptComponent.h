#pragma once
#include "BaseComponent.h"

class Script;

class ScriptComponent final : public BaseComponent
{
public:
	ScriptComponent(GameObject* pObject);
	~ScriptComponent();

	void Initialize() override;
	void LateInitialize() override;
	void Render() override;
	void Update(float elapsedSec) override;
	void DrawInterface() override;

	void SaveAttributes(rapidxml::xml_document<>* pDoc, rapidxml::xml_node<>* pNode) override;

	void SetAttributes(Script* pScript, rapidxml::xml_node<>* pNode);
	void SetScript(Script* pScript);
	Script* GetScript() const { return m_pScript; }

private:
	Script* m_pScript = nullptr;
};

