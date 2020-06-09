#pragma once
#include "BaseComponent.h"

class Script;

class ScriptComponent final : public BaseComponent
{
public:
	ScriptComponent(GameObject* pObject);
	~ScriptComponent();

	void Initialize() override;
	void Render() override;
	void Update(float elapsedSec) override;
	void DrawInterface() override;

	void SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node) override;

	void SetAttributes(Script* pScript);;
	void SetScript(Script* pScript) { if (pScript) m_pScript = pScript; }

private:
	Script* m_pScript{ nullptr };
};

