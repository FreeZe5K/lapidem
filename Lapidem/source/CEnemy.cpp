#include "CEnemy.h"
#include "CAnimation.h"
#include "IAIState.h"
#include "AIStateEarth.h"
#include "CTerrainBase.h"
#include "Corona_EventHandler.h"
#include "CSpell.h"
#include "Wrappers/CSGD_TextureManager.h"
#include "CGameplayState.h"

CEnemy::CEnemy(EleType ElementToBe, float initx, float inity)
{
	m_nType = OBJ_ENEMY;
	switch(ElementToBe)
	{
	case OBJ_EARTH:
		currState = new AIStateEarth();
		SetPosX(initx);
		SetPosY(inity);
		((AIStateEarth *)currState)->SetInitPos((int)GetPosX(), (int)GetPosY());
		SetImage( CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/lapidem_lulzenemy.png"));
		SetHeight(64);
		SetWidth(16);
		m_nHealth = 80;
		break;
	case OBJ_FIRE:
		break;
	case OBJ_ICE:
		break;
	case OBJ_WIND:
		break;
	}
	m_fShotTimer = 3.0f;
}

CEnemy::~CEnemy()
{
	if( GetImage() >= 0 )
	CSGD_TextureManager::GetInstance()->UnloadTexture( GetImage() );

}

void CEnemy::Update(float fElapsedTime)
{


	m_fShotTimer -= fElapsedTime;
	if(currState->Update(fElapsedTime, this) && m_fShotTimer <0)
	{
		currState->Attack(CGameplayState::GetInstance()->GetPlayerOne(), this);
		m_fShotTimer = 2.0f;
	}

	CCharacter::Update(fElapsedTime);

	if(m_nHealth <= 0)
	{
		Corona_EventHandler::GetInstance()->SendEvent("EnemyDied", (void*)this);
		SetActive(false);
	}

}

void CEnemy::HandleCollision(CBase* collidingObject)
{
	if( collidingObject->GetType() == OBJ_TERRA  )
	{
		int TerraType = ((CTerrainBase*)collidingObject)->GetTypeTerrain();

		RECT r;
		IntersectRect( &r, & this->GetCollisionRect(0), &collidingObject->GetCollisionRect(0) );

		int nRectWidth = r.right -r.left;
		int nRectHeight = r.bottom - r.top;

		if( nRectHeight > nRectWidth )
		{
			if( this->GetPosX() > collidingObject->GetPosX() )
				SetPosX( GetPosX() + nRectWidth );
			else if ( this->GetPosX() < collidingObject->GetPosX() )
				SetPosX( GetPosX() - nRectWidth );
		}
		else if( nRectHeight < nRectWidth ) 
		{
			
			if( this->GetPosY() > collidingObject->GetPosY() )
				SetPosY( GetPosY() + nRectHeight  );
			else if(this->GetPosY() < collidingObject->GetPosY() )
				SetPosY( GetPosY() - nRectHeight );

		}
	}
	else if(collidingObject->GetType() == OBJ_SPELL)
	{
		m_nHealth -= ((CSpell*)collidingObject)->GetDamage();
	}
}