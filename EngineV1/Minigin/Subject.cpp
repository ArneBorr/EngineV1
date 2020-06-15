#include "MiniginPCH.h"
#include "Subject.h"
#include "Observer.h"

void Subject::AddObserver(Observer* pObserver)
{
	auto it = std::find(m_pObservers.begin(), m_pObservers.end(), pObserver);
	if (it == m_pObservers.end())
		m_pObservers.push_back(pObserver);
}

void Subject::RemoveObserver(Observer* pObserver)
{
	auto it = std::find(m_pObservers.begin(), m_pObservers.end(), pObserver);
	if (it != m_pObservers.end())
		m_pObservers.erase(it);
}

void Subject::Notify(const std::string& event, GameObject* pObject, GameObject* pObjCollWith)
{
	for (auto pObserver : m_pObservers)
		pObserver->OnNotify(event, pObject, pObjCollWith);
}
