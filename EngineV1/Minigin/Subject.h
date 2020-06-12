#pragma once

class Observer;

class Subject
{
public:
	Subject() = default;

	void AddObserver(Observer* pObserver);
	void RemoveOberver(Observer* pObserver);
	void Notify(const std::string& event);

protected:
	std::vector<Observer*> m_pObservers{};

};

