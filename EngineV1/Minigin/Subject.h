#pragma once

class Observer;
class GameObject;

class Subject
{
public:
	Subject() = default;

	void AddObserver(Observer* pObserver);
	void RemoveObserver(Observer* pObserver);
	void Notify(const std::string& event, GameObject* pObject, GameObject* pObjCollWith);

protected:
	std::vector<Observer*> m_pObservers = {};
};

