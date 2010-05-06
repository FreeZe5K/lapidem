#pragma once

#include "CTerrainBase.h"
#include "CLDevice.h"
#include "Corona_EventHandler.h"

class CLevelSwitch : public CTerrainBase/*, public CLDevice*/
{
	bool m_bIsOn;
	int m_nSwitchModeID[2];
public:
	CLevelSwitch();
	~CLevelSwitch();

	void HandleCollision(CBase*);

	void SetSwitchState(bool bIsOn) { m_bIsOn = bIsOn; }
	bool GetSwitchState() { return m_bIsOn; }

	void Render();

	//void HandleEvent(CEvent*);
};