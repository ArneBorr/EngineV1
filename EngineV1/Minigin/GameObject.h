#pragma once
#include "SceneObject.h"

class Scene;
class BaseComponent;
class TransformComponent;
class RigidbodyComponent;

class GameObject : public SceneObject
{
public:
	GameObject(const std::string& name);
	virtual ~GameObject();

	GameObject(const GameObject& other) = delete;
	GameObject(GameObject&& other) noexcept;
	GameObject& operator=(const GameObject& other) = delete;
	GameObject& operator=(GameObject&& other) noexcept;

	virtual void Initialize();
	virtual void Update(float elapsedSec) override;
	virtual void LateUpdate() override;
	virtual void Render() const override;

	void SaveAttributes(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* node) override;

	void DrawInterfaceScene();
	virtual void DrawInterfaceComponents();

	void AddComponent(BaseComponent* pComponent);
	template <class T>
	T* GetComponent();
	const std::vector<BaseComponent*>& GetComponents() const { return m_pComponents; };
	void ChangeComponentOrder(BaseComponent* pBehindComp, unsigned int currentIndex);

	void AddChild(GameObject* pGameObject, GameObject* behindObject = nullptr);
	void DetachChild(GameObject* pGameObject);
	void DeleteChild(GameObject* pGameObject);
	const std::vector<GameObject*>& GetChildren() const {}
	void SetScene(Scene* pScene);
	Scene* GetScene() const;
	void SetParent(GameObject* pGameObject);
	GameObject* GetParent() const { return m_pParent; };

	void ChangeToFullScreen();

	const std::string& GetName() const { return m_Name; };
	void SetName(const std::string& name) { m_Name = name; };

	unsigned int GetIndexInHierarchy() const { return m_IndexInHierarchy; };
	void SetIndexInHierarchy(unsigned int index) { m_IndexInHierarchy = index; };

	void SetTransformChanged(bool changed);
	TransformComponent* GetTransform() const { return m_pTransform; };
	void SetTransform(TransformComponent* transform) { m_pTransform = transform; };

	RigidbodyComponent* GetRigidbody() const { return m_pRigidbody; }
	void SetRigidbody(RigidbodyComponent* pBody) { m_pRigidbody = pBody; }


private:
	std::vector<GameObject*> m_pChildren{};
	std::vector<BaseComponent*> m_pComponents{};
	std::pair<unsigned int, unsigned int> m_ToBeChangedComponents{ }; //Prevent crash from happening: Item would be added to vector while looping over this vector


	Scene* m_pScene{ nullptr };
	TransformComponent* m_pTransform{ nullptr };
	RigidbodyComponent* m_pRigidbody{ nullptr };
	GameObject* m_pParent{ nullptr };
	GameObject* m_pToBeAddedObject{ nullptr }; //Prevent crash from happening: Item would be added to vector while looping over this vector
	GameObject* m_pToBeDetachedChild{ nullptr }; // Beytter way to do this (Deletion child)
	std::string m_Name{ };

	unsigned int m_IndexInHierarchy{};
	bool m_NeedChangeComponents{ false };
	bool m_WantsToDeleteThis{ false };
	bool m_HasTransformChanged{ false };

	const static unsigned int MAX_COMPONENTS = 10;
};

template<class T>
inline T* GameObject::GetComponent()
{
	const type_info& ti = typeid(T);
	for (auto pComp : m_pComponents)
	{
		if (pComp && typeid(*pComp) == ti)
			return static_cast<T*>(pComp);
	}

	return nullptr;
}
