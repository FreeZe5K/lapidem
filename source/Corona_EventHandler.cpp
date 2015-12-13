#include "Corona_EventHandler.h"

Corona_EventHandler* Corona_EventHandler::GetInstance()
{
	static Corona_EventHandler C_EH;
	return &C_EH;
}

void Corona_EventHandler::RegisterClient(CLDevice* Client, EVENTID EventID)
{
	if(!Client || AlreadyRegistered(EventID, Client))
		return;

	m_pClients.insert( make_pair(EventID, Client));
}

void Corona_EventHandler::UnregisterClient(EVENTID EventID, CLDevice* Client)
{

	multimap<EVENTID, CLDevice*>::iterator hunter;
	multimap<EVENTID, CLDevice*>::iterator limit;
	

	hunter = m_pClients.equal_range(EventID).first;
	limit  = m_pClients.equal_range(EventID).second;

	while(hunter != limit)
	{
		if((*hunter).second == Client)
		{
			hunter = m_pClients.erase(hunter);
			break;
		}
		++hunter;
	}
}

bool Corona_EventHandler::AlreadyRegistered(EVENTID EventID, CLDevice* Client)
{

	multimap<EVENTID, CLDevice*>::iterator hunter;
	multimap<EVENTID, CLDevice*>::iterator limit;

	hunter = m_pClients.equal_range(EventID).first;
	limit  = m_pClients.equal_range(EventID).second;

	while(hunter != limit)
	{
		if((*hunter).second == Client)
		{
			
			return true;
		}
		++hunter;
	}

	return false;
}
void Corona_EventHandler::SendEvent(EVENTID EventID, void* Data1, void* Data2)
{
	CEvent newEvent(EventID, Data1, Data2);
	m_pEvents.push_back(newEvent);
}
void Corona_EventHandler::ShutDownSystem()
{
	m_pClients.clear();
	m_pEvents.clear();
}
void Corona_EventHandler::ProcessEvents()
{
	while(m_pEvents.size())
	{
		multimap<EVENTID, CLDevice*>::iterator hunter;
		multimap<EVENTID, CLDevice*>::iterator limit;

		hunter = m_pClients.equal_range(m_pEvents.front().GetEventID()).first;
		limit  = m_pClients.equal_range(m_pEvents.front().GetEventID()).second;

		while(hunter != limit)
		{
			(*hunter).second->HandleEvent(&(m_pEvents.front()));
			++hunter;
		}

		m_pEvents.pop_front();
	}
}