#include "AIStateWind.h"
#include "CEnemy.h"
#include "CFlock.h"
#include "CLevel.h"
#include "CTerrainBase.h"
#include "CPlayer.h"
#include "Corona_ObjectManager.h"
#include "CGameplayState.h"
#include "Lapidem_Math.h"


AIStateWind::AIStateWind()
{
	m_pFlock = NULL;
	int shudder = rand() % 15;
	m_fShudderTimer = (float)shudder/10.0f;
	m_fInfluence =75.0f;
	m_pTarget = NULL;
	m_bAttacking = false;
}
AIStateWind::~AIStateWind()
{
}

int AIStateWind::Update( float fElapsedTime, CEnemy* pEnemy)
{
	m_fShudderTimer -= fElapsedTime;
	if(m_pFlock != NULL)	// make sure i dont go splode
	{
		if(m_pFlock->InRadius(pEnemy))	// am i still hanging out with my friends.
		{
			if(m_pFlock->GetFlockSize() >1)	// to see if flying solo
			{
				if(m_pFlock->GetLeader() == pEnemy)	// to check if i need to update the flock itself
				{
					if(m_pFlock->IsAttacking() && !m_pFlock->GetShocked())
					{
						return AttackUpdate(m_pFlock->GetTarget(),pEnemy,fElapsedTime);
					}
					if(!m_pFlock->GetShocked())
					{
						if(m_fShudderTimer < 0.0f)	// oh look at the shiny pebble... let me adjust my velocity a little bit to see it better
						{							// prevents flock from having same velocities.
							if(rand() %2)
							{
								pEnemy->SetVelX(pEnemy->GetVelX() + (rand()% 25 + 10));
								pEnemy->SetVelY(pEnemy->GetVelY() + (rand()% 25 + 10));
								if(pEnemy->GetVelX() > 150)
								{
									pEnemy->SetVelX(150);
								}
								if(pEnemy->GetVelY() > 150)
								{
									pEnemy->SetVelY(150);
								}
							}
							else
							{
								pEnemy->SetVelX(pEnemy->GetVelX() - (rand()% 25 + 10));
								pEnemy->SetVelY(pEnemy->GetVelY() - (rand()% 25 + 10));
								if(pEnemy->GetVelX() < -150)
								{
									pEnemy->SetVelX(-150);
								}
								if(pEnemy->GetVelY() < -150)
								{
									pEnemy->SetVelY(-150);
								}
							}
							int shudder = rand() % 10;
							m_fShudderTimer = (float)(shudder/10.0f);
						}
						// check to see if they are in the radius and do stuff
						int player1dist = int(m_pFlock->CalculateDistance(pEnemy->GetPosX(), pEnemy->GetPosY(),CGameplayState::GetInstance()->GetPlayerOne()->GetPosX(),CGameplayState::GetInstance()->GetPlayerOne()->GetPosY()));
						int player2dist = 1000;
						if(CGameplayState::GetInstance()->GetPlayerTwo() != NULL)
						{
							player2dist = int(m_pFlock->CalculateDistance(pEnemy->GetPosX(), pEnemy->GetPosY(),CGameplayState::GetInstance()->GetPlayerTwo()->GetPosX(),CGameplayState::GetInstance()->GetPlayerTwo()->GetPosY()));
						}

						if(player1dist < player2dist)
						{
							if(player1dist < 200)
							{
								m_pFlock->SetAttacking(true);
								m_pFlock->SetTarget(CGameplayState::GetInstance()->GetPlayerOne());
							}
						}
						else
						{
							if(player2dist < 200)
							{
								m_pFlock->SetAttacking(true);
								m_pFlock->SetTarget(CGameplayState::GetInstance()->GetPlayerTwo());
							}
						}						
					}
					else
					{
						pEnemy->SetVelX(float(rand() %300 - 150));
						pEnemy->SetVelY(float(rand() %300 - 150));
					}
				}
				else if(!m_pFlock->IsAttacking())	// not attacking yet and not the leader
				{
					/*float myvelx = pEnemy->GetVelX();
					float myvely = pEnemy->GetVelY();

					float flockvelx = m_pFlock->GetLeader()->GetVelX();
					float flockvely = m_pFlock->GetLeader()->GetVelY();

					float diffvelx = flockvelx - myvelx;
					float diffvely = flockvely - myvely;*/

					//pEnemy->SetVelX(pEnemy->GetVelX() + ((int)diffvelx/2));
					//pEnemy->SetVelY(pEnemy->GetVelY() + ((int)diffvely/2));
					if(!m_pFlock->GetShocked())		// im not shocked i act normally
					{
						pEnemy->SetVelX(m_pFlock->GetAverageXVel(pEnemy->GetPosX(), pEnemy->GetPosY(), m_fInfluence));
						pEnemy->SetVelY(m_pFlock->GetAverageYVel(pEnemy->GetPosX(), pEnemy->GetPosY(), m_fInfluence));
						if(pEnemy->GetVelX() < -150)
						{
							pEnemy->SetVelX(-150);
						}
						else if(pEnemy->GetVelX() < -150)
						{
							pEnemy->SetVelX(-150);
						}
						if(pEnemy->GetVelY() < -150)
						{
							pEnemy->SetVelY(-150);
						}

						else if(pEnemy->GetVelY() < -150)
						{
							pEnemy->SetVelY(-150);
						}
					}
					else			// holy crap i am freakin shocked our leader died what do i do
					{
						pEnemy->SetVelX(float(rand() %300 - 150));
						pEnemy->SetVelY(float(rand() %300 - 150));
					}
				}
				else						// holy crap im attacking
				{
					return AttackUpdate(m_pFlock->GetTarget(),pEnemy, fElapsedTime);
				}
			}
			else if(m_fShudderTimer < 0.0f)	// flock of one bird
			{
				if(m_pFlock->IsAttacking())
				{
					return AttackUpdate(m_pFlock->GetTarget(),pEnemy,fElapsedTime);
				}
				else if(!m_pFlock->GetShocked())
				{
					if(rand() %2)
					{
						pEnemy->SetVelX(pEnemy->GetVelX() + (rand()% 25 + 10));
						pEnemy->SetVelY(pEnemy->GetVelY() + (rand()% 25 + 10));
					}
					else
					{
						pEnemy->SetVelX(pEnemy->GetVelX() - (rand()% 25 + 10));
						pEnemy->SetVelY(pEnemy->GetVelY() - (rand()% 25 + 10));
					}
					int shudder = rand() % 10;
					m_fShudderTimer = (float)(shudder/10.0f);

					int player1dist = int(m_pFlock->CalculateDistance(pEnemy->GetPosX(), pEnemy->GetPosY(),CGameplayState::GetInstance()->GetPlayerOne()->GetPosX(),CGameplayState::GetInstance()->GetPlayerOne()->GetPosY()));
					int player2dist = 1000;
					if(CGameplayState::GetInstance()->GetPlayerTwo() != NULL)
					{
						player2dist = int(m_pFlock->CalculateDistance(pEnemy->GetPosX(), pEnemy->GetPosY(),CGameplayState::GetInstance()->GetPlayerTwo()->GetPosX(),CGameplayState::GetInstance()->GetPlayerTwo()->GetPosY()));
					}

					if(player1dist < player2dist)
					{
						if(player1dist < 200)
						{
							m_pFlock->SetAttacking(true);
							m_pFlock->SetTarget(CGameplayState::GetInstance()->GetPlayerOne());
						}
					}
					else
					{
						if(player2dist < 200)
						{
							m_pFlock->SetAttacking(true);
							m_pFlock->SetTarget(CGameplayState::GetInstance()->GetPlayerTwo());
						}
					}	
				}
				else
				{
					pEnemy->SetVelX(float(rand() %300 - 150));
					pEnemy->SetVelY(float(rand() %300 - 150));
				}

			}
		}
		else		// im outside my flocks radius of influence... imma create my own flock
		{
			if(m_pFlock)
			{
				m_pTarget = m_pFlock->GetTarget();
				m_bAttacking = m_pFlock->IsAttacking();
			}
			m_pFlock->RemoveMember(pEnemy);
			m_pFlock = NULL;
			/* MAY NOT NEED THIS
			m_pFlock = new CFlock();
			m_pFlock->AddMember(pEnemy);
			m_pFlock->SetPosX(pEnemy->GetPosX());
			m_pFlock->SetPosY(pEnemy->GetPosY());*/
		}
	}
	else //aww man... i have no flock... ill create my own.
	{
		m_pFlock = new CFlock();
		m_pFlock->AddMember(pEnemy);
		m_pFlock->SetAttacking(m_bAttacking);
		m_pFlock->SetTarget(m_pTarget);
		m_pFlock->SetPosX(pEnemy->GetPosX());
		m_pFlock->SetPosY(pEnemy->GetPosY());
		Corona_ObjectManager::GetInstance()->AddObject(m_pFlock);
		m_pFlock->Release();
	}

	//if(pEnemy->GetPosX() < 0 ||
	return 0;
}
void AIStateWind::Attack( CCharacter* pTarget, CCharacter* pEnemy)
{
	CSpellFactory::GetInstance()->CreateEnemyWind(pEnemy,pTarget);
}
bool AIStateWind::CheckPassable(CLevel* pLevel, CBase* pObject, float fElapsedTime)
{
	int nTilesToCheck = pObject->GetHeight() / pLevel->GetTileHeight();

	int nDistanceToNext = pObject->GetVelX() > 0 ? (pObject->GetWidth() + (pLevel->GetTileWidth()>>1)) : -(pLevel->GetTileWidth()>>1);

	int nPosX = (int)(pObject->GetPosX() + nDistanceToNext + pObject->GetVelX() * fElapsedTime);
	int nPosY = (int)pObject->GetPosY();
	CTerrainBase* pTerrain;

	for(int i = 0; i < nTilesToCheck; ++i)
	{
		pTerrain = (CTerrainBase*)pLevel->GetTile(nPosX, nPosY);

		if(pTerrain->GetType() != OBJ_EVENT)
		{
			if(pTerrain->GetTypeTerrain() != T_EMPTY)
				return false;
		}
		nPosY += pLevel->GetTileHeight();
	}
	return true;
}

int AIStateWind::AttackUpdate(CPlayer* pTarget, CEnemy* pEnemy, float fElapsedTime)
{
	tVector2D enemypos;
	enemypos._x= pEnemy->GetPosX();
	enemypos._y= pEnemy->GetPosY();

	tVector2D targetpos;
	targetpos._x= pTarget->GetPosX();
	targetpos._y= pTarget->GetPosY() - 30;


	tVector2D enemyvelocity;
	enemyvelocity._x = pEnemy->GetVelX()/*100.0f*/;
	enemyvelocity._y = pEnemy->GetVelY() /*100.0f*/;

	float angle = Lapidem_Math::GetInstance()->AngleBetweenVectors(targetpos,enemypos);

	enemyvelocity = Lapidem_Math::GetInstance()->Vector2DRotate(enemyvelocity,angle);

	if(pEnemy->GetPosX() > pTarget->GetPosX())
	{
		pEnemy->SetVelX(enemyvelocity._x/*pEnemy->GetVelX() + enemyvelocity._x * fElapsedTime * 100*/);
		pEnemy->SetVelY(enemyvelocity._y/*pEnemy->GetVelY() + enemyvelocity._y * fElapsedTime * 100*/);
	}
	else
	{
		pEnemy->SetVelX(-enemyvelocity._x/*pEnemy->GetVelX() + enemyvelocity._x * fElapsedTime*/);
		pEnemy->SetVelY(enemyvelocity._y/*pEnemy->GetVelY() + enemyvelocity._y * fElapsedTime*/);
	}


	if(pEnemy->GetVelX() > 100.0f)
	{
		pEnemy->SetVelX(100.0f);
	}
	else if(pEnemy->GetVelX() < -100.0f)
	{
		pEnemy->SetVelX(-100.0f);
	}
	if(pEnemy->GetVelY() > 100.0f)
	{
		pEnemy->SetVelY(100.0f);
	}
	else if(pEnemy->GetVelY() < -100.0f)
	{
		pEnemy->SetVelY(-100.0f);
	}

	if(pTarget == CGameplayState::GetInstance()->GetPlayerOne())
	{
		return 1;
	}
	else if(pTarget ==CGameplayState::GetInstance()->GetPlayerTwo())
	{
		return 2;
	}
	return 0;
}