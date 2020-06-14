#pragma once
#include "Script.h"

class RigidbodyComponent;

class PickUp final : public Script
{
public:
	PickUp();

	void Initialize() override;
	void DrawInterface() override;
	void OnNotify(const std::string& event, GameObject* pObject, GameObject* collWithObj) override;
	virtual void SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node);
	virtual void SetAttributes(rapidxml::xml_node<>*);

	float GetPoints() const { return m_Points; };
private:
	RigidbodyComponent* m_pRigidbody{ nullptr };
	float m_Points{};
	bool m_IsCollisionIgnored{ false };
};

