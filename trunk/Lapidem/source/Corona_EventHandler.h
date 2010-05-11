#pragma once

#include <list>
#include <map>
using namespace std;

#include "CLDevice.h"


class Corona_EventHandler
{
	multimap<EVENTID, CLDevice*> m_pClients;
	list<CEvent>				 m_pEvents;

	Corona_EventHandler() {}
	Corona_EventHandler(const Corona_EventHandler &) {}
	Corona_EventHandler& operator=(const Corona_EventHandler&) {}
	~Corona_EventHandler() {};

	bool AlreadyRegistered(EVENTID EventID, CLDevice* Client);
public:
	static Corona_EventHandler * GetInstance();
	void RegisterClient(CLDevice* Client, EVENTID EventID);
	void UnregisterClient(EVENTID EventID, CLDevice* Client);
	void SendEvent(EVENTID EventID, void* Data1 = NULL, void* Data2 = NULL);
	void ProcessEvents();
	void ShutDownSystem();



};