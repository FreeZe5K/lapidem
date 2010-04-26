#pragma once
#include "CTerrainBase.h"
#include "CLDevice.h"
#include "Corona_EventHandler.h"

class CEnemy;

class CEnemySpawner : public CTerrainBase , public CLDevice
{
	CEnemy* m_pEnemy;
	bool m_bIsReadyToSpawn;
	bool m_bIsOnScreen;

public:
	CEnemySpawner() { m_pEnemy = NULL; m_bIsReadyToSpawn = true; m_bIsOnScreen = false;
	Corona_EventHandler::GetInstance()->RegisterClient(this, "OffScreenPulse");
	Corona_EventHandler::GetInstance()->RegisterClient(this, "EnemyDied");}
	~CEnemySpawner() {Corona_EventHandler::GetInstance()->UnregisterClient("OffScreenPulse", this);
					  Corona_EventHandler::GetInstance()->UnregisterClient("EnemyDied", this);}

	void Update(float fElapsedTime) {m_bIsOnScreen = true;}
	void Render()					{m_bIsOnScreen = false;}

	void HandleEvent(CEvent* pEvent);

};