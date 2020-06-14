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

	void SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node) override;

	void SetAttributes(Script* pScript, rapidxml::xml_node<>* node);
	void SetScript(Script* pScript);
	Script* GetScript() const { return m_pScript; }

private:
	Script* m_pScript{ nullptr };
};

