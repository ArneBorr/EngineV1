#pragma once
#include "SceneObject.h"

class BaseComponent;
class GameObject final : public SceneObject
{
public:
	GameObject(const std::string& name);
	virtual ~GameObject();

	GameObject(const GameObject& other) = delete;
	GameObject(GameObject&& other) = delete;
	GameObject& operator=(const GameObject& other) = delete;
	GameObject& operator=(GameObject&& other) = delete;

	void Update(float elapsedSec) override;
	void Render() const override;

	void DrawInterfaceScene();
	void DrawInterfaceComponents();

	void AddComponent(BaseComponent* pComponent);
	template <class T>
	T* GetComponent();
	const std::string& GetName() const { return m_Name; };
	void SetName(const std::string& name) { m_Name = name; };

private:
	std::vector<BaseComponent*> m_pComponents;
	std::string m_Name{};

	bool m_Selected{ false };

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
