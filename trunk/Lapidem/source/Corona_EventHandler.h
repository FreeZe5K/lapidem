#pragma once
#include "CLDevice.h"

#include <list>
#include <map>
using namespace std::multimap;
using namespace std::pair;
using namespace std::list;


class Corona_EventHandler
{
	multimap<EVENTID, CLDevice*> m_pClients;
	List<EVENTID>				 m_pEvents;

	Corona_EventHandler() {}
	Corona_EventHandler(const Corona_EventHandler &) {}
	Corona_EventHandler& operator=(const Corona_EventHandler&) {}
	~Corona_EventHandler() {};

	bool AlreadyRegistered(EVENTID EventID, CLDevice* Client);
	void ClearEvents();
public:
	void RegisterClient(CLDevice* Client, EVENTID EventID);






};