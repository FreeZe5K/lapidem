#include "CEnemySpawner.h"
#include "Corona_ObjectManager.h"
#include "CEnemy.h"
#include "CFlock.h"

#define WIND_SPAWN 8
CEnemySpawner::CEnemySpawner()
{ 
		m_pEnemy            = NULL; 
		m_bIsReadyToSpawn   = true; 
		m_bIsOnScreen       = false;
		Corona_EventHandler::GetInstance( )->RegisterClient( this, "OffScreenPulse" );
		Corona_EventHandler::GetInstance( )->RegisterClient( this, "EnemyDied" );
}

void CEnemySpawner::Update(float fElapsedTime)
{
	if(Corona_ObjectManager::GetInstance()->IsOnScreen(this))
		m_bIsOnScreen = true;
	else
		m_bIsOnScreen = false;

	if(m_bIsReadyToSpawn && !m_bIsOnScreen)
	{
		int typetospawn = (rand() % 4);

			if(typetospawn != OBJ_WIND)
			{
				m_pEnemy = new CEnemy((EleType)typetospawn, GetPosX(), GetPosY(),false, NULL);
				Corona_ObjectManager::GetInstance()->AddObject(m_pEnemy);
			}
			else 
			{
				CFlock* pleasework = new CFlock();
				for(int i =0; i < WIND_SPAWN; i++)
				{
					if(m_pEnemy != NULL)
					{
						m_pEnemy->Release();
					}
					m_pEnemy = new CEnemy((EleType)typetospawn, GetPosX() + i*20, GetPosY() + (20 * i%4)  ,false, pleasework);
					pleasework->AddMember(m_pEnemy);
					Corona_ObjectManager::GetInstance()->AddObject(m_pEnemy);
				}
				Corona_ObjectManager::GetInstance()->AddObject(pleasework);
				pleasework->Release();
			}
			m_bIsReadyToSpawn = false;
	}

	if(m_pEnemy && !m_pEnemy->IsActive()) //Backup: Coded when Enemy spawners were being destroyed
										  // Should never be true.
	{
		m_pEnemy->Release();
		m_pEnemy = NULL;
		m_bIsReadyToSpawn = true;
	}
}


void CEnemySpawner::HandleEvent(CEvent* pEvent)
{

	if(!strcmp(pEvent->GetEventID().c_str(), "EnemyDied") && pEvent->GetData1() == m_pEnemy)
	{
		if(m_pEnemy)
			m_pEnemy->Release();
		m_pEnemy = NULL;
		m_bIsReadyToSpawn = true;
	}
	else if(!strcmp(pEvent->GetEventID().c_str(), "EnemyDied") && pEvent->GetData1() == NULL)
	{
		if(m_pEnemy)
			m_pEnemy->Release();
		m_pEnemy = NULL;
	}
}