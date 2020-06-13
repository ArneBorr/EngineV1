#pragma once
#include "BaseComponent.h"
#include "RigidbodyComponent.h"


class Texture2D;

class BoxColliderComponent : public BaseComponent
{
public:
	BoxColliderComponent(GameObject* pObject);
	~BoxColliderComponent();

	void Render() override;
	void Update(float elapsedSec) override;
	void DrawInterface() override;

	void SaveAttributes(rapidxml::xml_document<>* doc, rapidxml::xml_node<>* node) override;
	void SetAttributes(const std::vector<bool>& ignoreGroups, const Vector2f& offset, float width, 
		float height, float density, float friction, float restitution, int collGroup, int renderCollider, int isSensor);

	void SetIgnoreGroups(const std::vector<bool>& ignoreGroups);
	void SetIgnoreGroup(int i, bool ignore);
	void SetRestitution(float restitution) { m_pFicture->SetRestitution(restitution); };

	void CreateLink(RigidbodyComponent* pBody);
	b2Fixture* Unlink();
	void LoadSettings(const std::string& settings);

	void CreateShape();
	void RegisterCollision(GameObject* pObject, GameObject* collWith, bool begin);

private:
	enum CollisionGroup : uint16 { // Not enum class since that does not work with box2d
		None = 0x0000,
		One = 0x0001,
		Two = 0x0002,
		Three = 0x0004,
		Four = 0x0008,
		Five = 0x0010,
	};

	static const int m_NrOfCollGroups = 5;

	RigidbodyComponent* m_pRigidbody{ nullptr };
	Texture2D* m_pTexture{ nullptr };
	b2Fixture* m_pFicture{ nullptr };

	CollisionGroup m_CollisionGroup{ CollisionGroup::One };
	std::string m_CollisionItems[m_NrOfCollGroups]{}; // ImGui

	Vector2f m_Offset{};
	bool m_IgnoreGroups[m_NrOfCollGroups]{};
	float m_Density{ 10.f };
	float m_Friction{ 0.65f };
	float m_Restitution{ 0.3f };
	float m_Width{ 200.f };
	float m_Height{ 200.f };
	int m_SelectedCollGroupIndex{ 0 };
	bool m_RenderCollider{ true };
	bool m_IsSensor{ false };

	CollisionGroup GetCollGroup(int i);
	b2Filter GetFilter();
	void SetCollisionGroups();
	void LoadPlayerSettings(bool overlap);
	void LoadBubbleSettings(bool overlap);
	void LoadZenChanSettings();
	void LoadFriesSettings(bool overlap);
};

