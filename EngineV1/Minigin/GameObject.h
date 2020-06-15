#pragma once
#include "rapidxml.hpp"
class Scene;
class BaseComponent;
class TransformComponent;
class RigidbodyComponent;

class GameObject final
{
public:
	GameObject(const std::string& name);
	virtual ~GameObject();

	GameObject(const GameObject& other) = delete;
	GameObject(GameObject&& other) noexcept;
	GameObject& operator=(const GameObject& other) = delete;
	GameObject& operator=(GameObject&& other) = delete;

	void Initialize();
	void LateInitialize();
	void Update(float elapsedSec);
	void LateUpdate();
	void Render() const;
	void Reset();

	void SaveAttributes(rapidxml::xml_document<>* pDoc, rapidxml::xml_node<>* Node);

	void DrawInterfaceScene();
	virtual void DrawInterfaceComponents();

	void AddComponent(BaseComponent* pComponent);
	template <class T>
	T* GetComponent();
	const std::vector<BaseComponent*>& GetComponents() const { return m_pComponents; };
	template <class T>
	std::vector<T*> GetComponents() const;
	void ChangeComponentOrder(BaseComponent* pBehindComp, unsigned int currentIndex);

	void AddChild(GameObject* pGameObject, GameObject* pBehindObject = nullptr);
	void DetachChild(GameObject* pGameObject);
	void DeleteChild(GameObject* pGameObject);
	void DetachThis();

	const std::vector<GameObject*>& GetChildren() const {}
	void SetParent(GameObject* pGameObject);
	GameObject* GetParent() const { return m_pParent; };

	void SetScene(Scene* pScene);
	Scene* GetScene() const;

	void AdaptToFullScreen(const Vector2f& ratio);
	bool HasTransformChanged() const { return m_HasTransformChanged; }

	auto GetName() const { return m_Name; };
	void SetName(const std::string& name) { strcpy_s(m_Name, name.c_str()); };

	unsigned int GetIndexInHierarchy() const { return m_IndexInHierarchy; };
	void SetIndexInHierarchy(unsigned int index) { m_IndexInHierarchy = index; };

	void SetTransformChanged(bool changed);
	TransformComponent* GetTransform() const { return m_pTransform; };
	void SetTransform(TransformComponent* pTransform) { m_pTransform = pTransform; };

	RigidbodyComponent* GetRigidbody() const { return m_pRigidbody; }
	void SetRigidbody(RigidbodyComponent* pBody) { m_pRigidbody = pBody; }

	void AddTag(const std::string& tag);
	bool HasTags(const std::vector<std::string>& tags);
	const std::vector<std::string>& GetTags() const { return m_Tags; }

private:
	const static unsigned int MAX_COMPONENTS = 10;

	std::vector<GameObject*> m_pChildren = {};
	std::vector<BaseComponent*> m_pComponents = {};
	std::vector<std::string> m_Tags = {};
	std::pair<unsigned int, unsigned int> m_ToBeChangedComponents = { }; //Prevent crash from happening: Item would be added to vector while looping over this vector


	Scene* m_pScene = nullptr;
	TransformComponent* m_pTransform = nullptr;
	RigidbodyComponent* m_pRigidbody = nullptr;
	GameObject* m_pParent = nullptr;
	GameObject* m_pToBeAddedObject = nullptr; //Prevent crash from happening: Item would be added to vector while looping over this vector
	GameObject* m_pToBeDetachedChild = nullptr; 
	char m_Name[100]{ };

	unsigned int m_IndexInHierarchy = {};
	bool m_NeedChangeComponents = false;
	bool m_WantsToDeleteThis = false;
	bool m_HasTransformChanged = false;

	void ChangeHierarchy();
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

template<class T> 
inline std::vector<T*> GameObject::GetComponents() const
{
	std::vector<T*> components{};

	const type_info& ti = typeid(T);
	for (auto pComp : m_pComponents)
	{
		if (pComp && typeid(*pComp) == ti)
			components.push_back(static_cast<T*>(pComp));
	}

	return components;
}
