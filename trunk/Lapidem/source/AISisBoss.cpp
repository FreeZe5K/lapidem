#include "AISisBoss.h"
#include "CGameplayState.h"
#include "CTerrainBase.h"

#include "AIStateEarth.h"
#include "AIStateFire.h"
#include "AIStateIce.h"


AISisBoss::AISisBoss()
{
	m_nInitX = m_nInitY = 0;
	m_bIsJumping = false;
	m_nCurrentElement = OBJ_EARTH;
	m_fVulnerableTimer = ATTACK_TIME;
	m_pTarget = CGameplayState::GetInstance()->GetPlayerOne();
	m_fAttackCooldown = 0.0f;
	m_fBurstCooldown = 0.0f;

	m_pStates[0] = new AIStateFire;
	m_pStates[1] = new AIStateIce;
	m_pStates[2] = this;
	m_pStates[3] = new AIStateEarth;
}

AISisBoss::~AISisBoss()
{
	for(int i = 0; i < 4; ++i)
	{
		if(m_pStates[i] != this)
			delete m_pStates[i];
	}
}

int AISisBoss::Update(float fElapsedTime, CEnemy* pEnemy)
{
	int ReturnVal = 0;
	CGameplayState* pGameState = CGameplayState::GetInstance();

	m_fVulnerableTimer -= fElapsedTime;
	m_fBurstCooldown += fElapsedTime;
	
	if(m_fVulnerableTimer <= 0.0f)
	{
		if(pGameState->GetPlayerTwo())
		{
			if(rand() % 2)
				m_pTarget = pGameState->GetPlayerTwo();
			else
				m_pTarget = pGameState->GetPlayerOne();
		}
		m_nCurrentElement = m_pTarget->GetEleType();
		pEnemy->SetEleType((EleType)m_nCurrentElement);
		m_fVulnerableTimer = ATTACK_TIME;
	}

	if(m_fVulnerableTimer <= ATTACK_TIME - V_TIME)	// Time to attack
	{
		if(m_fBurstCooldown >= BURST_TIMER)
		{
			//CreateElementBurst(pEnemy);
			m_fBurstCooldown = 0.0f;
		}
		if(m_pStates[m_nCurrentElement] != this)
			return m_pStates[m_nCurrentElement]->Update(fElapsedTime, pEnemy);
		else
		{
			m_fAttackCooldown += fElapsedTime;

			HandleMovement(fElapsedTime, pEnemy);
			CLevel* pLevel = pGameState->GetLevel();

			float posx, posy;
			posx = pGameState->GetPlayerOne( )->GetPosX( );
			posy = pGameState->GetPlayerOne( )->GetPosY( );

			posx = posx - pEnemy->GetPosX( );
			posy = posy - pEnemy->GetPosY( );

			posx = posx * posx;
			posy = posy * posy;

			posx = posx + posy;

			CPlayer* two = CGameplayState::GetInstance( )->GetPlayerTwo( );
			float dist2( 1000 );

			if( two )
			{
				float posx2 = two->GetPosX() - pEnemy->GetPosX();
				float posy2 = two->GetPosY() - pEnemy->GetPosY();

				posy2 = posy2 * posy2;
				posx2 = posx2 * posx2;

				posx2 += posy2;

				dist2 = sqrt(posx2);
			}

			float dist = sqrt( posx );

			if( dist2 < dist && dist2 < 300 )
			{
				if( dist2 < 100 )
				{
					if( two->GetPosX( ) > pEnemy->GetPosX( ) )
						pEnemy->SetVelX( -150.0f );
					else if( two->GetPosX( ) > pEnemy->GetPosX( ) )
						pEnemy->SetVelX( 150.0f );
				}

				pEnemy->SetVelX( 0 );
				ReturnVal = 2;
			}
			else if( dist < 300 )
			{
				CPlayer* one = pGameState->GetPlayerOne( );
				if( dist < 100 )
				{
					if( one->GetPosX( ) > pEnemy->GetPosX( ) )
						pEnemy->SetVelX( -150.0f );
					else if( one->GetPosX( ) > pEnemy->GetPosX( ) )
						pEnemy->SetVelX( 150.0f );
				} 
				ReturnVal = 1;
			}

		} // if Element is this
	}	// if able to attack
	else
		ReturnVal = 0;

	return ReturnVal;
}

void AISisBoss::Attack(CCharacter *pTarget, CCharacter *pAttacker)
{
	if(m_pStates[m_nCurrentElement] != this)
		m_pStates[m_nCurrentElement]->Attack(pTarget, pAttacker);
	else
	{
		if(pTarget)
		{
			if(m_fAttackCooldown >= ATTACK_RATE)
			{
				CSpellFactory::GetInstance()->CreateEnemyWind(pAttacker, pTarget);
				m_fAttackCooldown = 0.0f;
			}
		}
	}
}

void AISisBoss::HandleMovement(float fElapsedTime, CEnemy* pEnemy)
{
	CLevel* pLevel = CGameplayState::GetInstance()->GetLevel();
	//CTerrainBase* pTerrain;

	pEnemy->SetVelY(pEnemy->GetVelY() + 300 * fElapsedTime);
	if(!m_bIsJumping)
	{
		if(!CheckPassable(pLevel, pEnemy, fElapsedTime))
		{
			Jump();
			pEnemy->SetPosY(pEnemy->GetPosY() - 20);
			pEnemy->SetVelX((float)(150 * GetJumpDirection(pEnemy, fElapsedTime)));
			pEnemy->SetVelY(-200);
		}
	}

	if(IsLanded(pEnemy, fElapsedTime))
	{
		ResetJump();
		pEnemy->SetVelY(0.0f);
	}

}

bool AISisBoss::CheckPassable(CLevel* pLevel, CBase* pObject, float fElapsedTime)
{
	int nDistanceToNext = pObject->GetVelX() > 0 ? (pObject->GetWidth() + (pLevel->GetTileWidth()>>1)) : -(pLevel->GetTileWidth()>>1);

	int nBottomX = (int)pObject->GetPosX() + nDistanceToNext + (int)(pObject->GetVelX() * fElapsedTime);
	int nBottomY = (int)pObject->GetPosY() + pObject->GetHeight();
	CTerrainBase* pTerrain = (CTerrainBase*)pLevel->GetTile(nBottomX, nBottomY);
	if(pTerrain && pTerrain->GetType() == OBJ_TERRA && pTerrain->GetTypeTerrain() == T_EMPTY)
		return true;

	return false;
}

bool AISisBoss::IsLanded(CCharacter* pObject, float fElapsedTime)
{
	CLevel* pLevel = CGameplayState::GetInstance()->GetLevel();

	int nPosX = (int)pObject->GetPosX() + (pObject->GetWidth()>>1);
	int nPosY = (int)pObject->GetPosY()/* + pObject->GetHeight()*/ + (int)(pObject->GetVelY() * fElapsedTime);

	CTerrainBase* pTerrain = (CTerrainBase*)pLevel->GetTile(nPosX, nPosY);
	
	if(pTerrain)
	{
		if(pTerrain->GetType() == OBJ_TERRA && pTerrain->GetTypeTerrain() == T_ROCK)
			return true;
	}

	return false;
}

int AISisBoss::GetJumpDirection(CCharacter* pObject, float fElapsedTime)
{
	int nNumTiles = 2;
	
	CLevel* pLevel = CGameplayState::GetInstance()->GetLevel();
	CTerrainBase* pTerrain;

	int nPosX = (int)pObject->GetPosX() + (pObject->GetWidth()>>1) + pLevel->GetTileWidth();
	int nPosY = (int)pObject->GetPosY();

	for(int i = 0; i < nNumTiles; ++i)
	{
		pTerrain = (CTerrainBase*)pLevel->GetTile(nPosX, nPosY);
		if(pTerrain)
			if(pTerrain->GetType() == OBJ_TERRA && pTerrain->GetTypeTerrain() == T_EMPTY)
				return 1;

		nPosY -= pLevel->GetTileHeight();
	}

	nPosX = (int)pObject->GetPosX() - pLevel->GetTileWidth();
	nPosY = (int)pObject->GetPosY();

	for(int i = 0; i < nNumTiles; ++i)
	{
		pTerrain = (CTerrainBase*)pLevel->GetTile(nPosX, nPosY);
		if(pTerrain)
			if(pTerrain->GetType() == OBJ_TERRA && pTerrain->GetTypeTerrain() == T_EMPTY)
				return -1;

		nPosY -= pLevel->GetTileHeight();
	}

	return 0;
}

//void AISisBoss::CreateElementBurst(CCharacter *pObject)
//{
//	switch(m_nCurrentElement)
//	{
//	case OBJ_FIRE:
//		break;
//	case OBJ_ICE:
//		break;
//	case OBJ_WIND:
//		break;
//	case OBJ_EARTH:
//		break;
//	}
//}