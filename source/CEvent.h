#pragma once
#include <string>
using namespace std;
#define EVENTID string


class CEvent
{
	EVENTID m_pID;
	void * m_pData1;
	void * m_pData2;

public:
	CEvent(EVENTID pID, void * pData1 = 0, void * pData2 = 0) {m_pID = pID; m_pData1 = pData1; m_pData2 = pData2;}

	EVENTID GetEventID() {return m_pID;}
	void * GetData1()	 {return m_pData1;}
	void * GetData2()    {return m_pData2;}
};