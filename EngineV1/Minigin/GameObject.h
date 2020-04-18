#pragma once
#include "SceneObject.h"

class BaseComponent;
class GameObject final : public SceneObject
{
public:
	GameObject(const std::string& name);
	virtual ~GameObject();

	GameObject(const GameObject& other) = delete;
	GameObject(GameObject&& other) noexcept;
	GameObject& operator=(const GameObject& other) = delete;
	GameObject& operator=(GameObject&& other) noexcept;

	void Update(float elapsedSec) override;
	void Render() const override;

	void DrawInterfaceScene();
	void DrawInterfaceComponents();

	void AddComponent(BaseComponent* pComponent);
	template <class T>
	T* GetComponent();

	void AddChild(GameObject* pGameObject, GameObject* behindObject = nullptr);
	void DetachChild(GameObject* pGameObject);
	void SetParent(GameObject* pGameObject);

	const std::string& GetName() const { return m_Name; };
	void SetName(const std::string& name) { m_Name = name; };

	unsigned int GetIndexInHierarchy() const { return m_IndexInHierarchy; };
	void SetIndexInHierarchy(unsigned int index) { m_IndexInHierarchy = index; };

private:
	std::vector<GameObject*> m_pChilds{};
	std::vector<BaseComponent*> m_pComponents{};

	GameObject* m_pToBeAddedObject{ nullptr }; //Prevent crash from happening: Item would be added to vector while looping over this vector

	GameObject* m_pParent{ nullptr };
	std::string m_Name{ };

	unsigned int m_IndexInHierarchy{};

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
