#pragma once

#include "CTerrainBase.h"
#include "CLDevice.h"
#include "Corona_EventHandler.h"

class CEnemy;

class CEnemySpawner : public CTerrainBase, public CLDevice
{
private:
	CEnemy*  m_pEnemy;
	bool     m_bIsReadyToSpawn;
	bool     m_bIsOnScreen;

public:
	CEnemySpawner( ) ;
	
	~CEnemySpawner( ) 
	{
		Corona_EventHandler::GetInstance( )->UnregisterClient( "OffScreenPulse", this );
		Corona_EventHandler::GetInstance( )->UnregisterClient( "EnemyDied", this );
	}

	void Update(float fElapsedTime);

	void HandleEvent( CEvent* );
};