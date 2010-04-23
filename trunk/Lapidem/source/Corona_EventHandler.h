#pragma once

#include <list>
#include <map>
using namespace std;

#include "CLDevice.h"


class Corona_EventHandler
{
	multimap<EVENTID, CLDevice*> m_pClients;
	list<EVENTID>				 m_pEvents;

	Corona_EventHandler() {}
	Corona_EventHandler(const Corona_EventHandler &) {}
	Corona_EventHandler& operator=(const Corona_EventHandler&) {}
	~Corona_EventHandler() {};

	bool AlreadyRegistered(EVENTID EventID, CLDevice* Client);
	void ClearEvents();
public:
	void RegisterClient(CLDevice* Client, EVENTID EventID);






};