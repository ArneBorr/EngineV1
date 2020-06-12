#pragma once
#include "BaseComponent.h"

class b2Body;
class BoxColliderComponent;



class RigidbodyComponent : public BaseComponent
{
public:
	RigidbodyComponent(GameObject* pObject);
	~RigidbodyComponent();

	void Render() override;
	void Update(float elapsedSec) override;
	void DrawInterface() override;

	void SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node) override;
	void SetAttributes(const std::string& type, bool fixedRot);

	b2Fixture* AddShape(const b2FixtureDef& fictureDef);
	void DestroyShape(b2Fixture* ficture);

	void SetPosition(const Vector2f& pos);
	Vector2f GetPosition() const;

	void SetRotation(float rotation);
	float GetRotation() const { return m_pBody->GetAngle() * 180 / M_PI; }; // In Degrees

	Vector2f GetVelocity() const;
	void SetVelocity(const Vector2f& vel);

	void SetIgnoreGroup(int group, bool ignore);
	void AddCollider(BoxColliderComponent* pCollider);

	void UpdateShapeScale();

	void LoadSettings(const std::string& settings);
	void Move(const Vector2f& vel, const Vector2f& maxVel);
	void Jump(float strength);

private:
	std::vector<BoxColliderComponent*> m_pColliders{};

	b2Body* m_pBody{ nullptr };

	int m_TypeButtonIndex{ 0 };
	bool m_HasFixedRotation{ false };

	void LoadPlayerSettings();
	void LoadBubbleSettings();
	void LoadZenChanSettings();
};

