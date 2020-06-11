#pragma once
#include "BaseComponent.h"

class b2Body;
class BoxColliderComponent;



class RigidbodyComponent : public BaseComponent
{
public:
	enum CollisionGroup : uint16 {
		None = 0x0000,
		One = 0x0001,
		Two = 0x0002,
		Three = 0x0004,
		Four = 0x0008,
		Five = 0x0010,
	};

	RigidbodyComponent(GameObject* pObject);
	~RigidbodyComponent();

	void Render() override;
	void Update(float elapsedSec) override;
	void DrawInterface() override;

	void SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node) override;
	void SetAttributes(const std::vector<bool>& ignoreGroups, const std::string& type, float density, float friction, float restitution, int collGroup, bool fixedRot);
	void ChangeShape(BoxColliderComponent* pBox, const b2PolygonShape& shape);

	void SetPosition(const Vector2f& pos);
	Vector2f GetPosition() const;
	void SetRotation(float rotation);
	float GetRotation() const { return m_pBody->GetAngle() * 180 / M_PI; }; // In Degrees
	Vector2f GetVelocity();
	void UpdateShapeScale();

	void SetIgnoreGroups(const std::vector<bool>& ignoreGroups);
	void SetIgnoreGroup(int i, bool ignore);
	const static int GetAmountOfCollGroups() { return m_NrOfCollGroups; }

	void LoadSettings(const std::string& settings);

private:
	friend class MovementComponent;
	friend class RunBehaviour;
	friend class JumpBehaviour;

	static const int m_NrOfCollGroups = 5;

	void Move(const Vector2f& vel, const Vector2f& maxVel);
	void Jump(float strength);
	void SetCollisionGroups();
	CollisionGroup GetCollGroup(int i);
	b2Filter GetFilter();
	void LoadPlayerSettings();

	b2Body* m_pBody{ nullptr };
	b2Fixture* m_pFicture{ nullptr };

	CollisionGroup m_CollisionGroup{ One };
	std::string m_CollisionItems[m_NrOfCollGroups]{}; // ImGui

	float m_Density{ 10.f };
	float m_Friction{ 0.65f };
	float m_Restitution{ 0.3f };
	bool m_NotIgnoreGroups[m_NrOfCollGroups]{};
	int m_TypeButtonIndex{ 0 };
	int m_SelectedCollGroupIndex{ 0 };
	bool m_HasFixedRotation{ false };
};

