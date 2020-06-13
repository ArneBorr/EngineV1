#include "MiniginPCH.h"
#include "Subject.h"
#include "Observer.h"

void Subject::AddObserver(Observer* pObserver)
{
	auto it = std::find(m_pObservers.begin(), m_pObservers.end(), pObserver);
	if (it == m_pObservers.end())
		m_pObservers.push_back(pObserver);
	else
		std::printf("Subject::AddObserver() : Already an observer");
}

void Subject::RemoveOberver(Observer* pObserver)
{
	auto it = std::find(m_pObservers.begin(), m_pObservers.end(), pObserver);
	if (it != m_pObservers.end())
		m_pObservers.erase(it);
	else
		std::printf("Subject::RemoveOberver() : Observer not found");
}

void Subject::Notify(const std::string& event, GameObject* pObject, GameObject* trigger)
{
	for (auto pObserver : m_pObservers)
		pObserver->OnNotify(event, pObject, trigger);
}
