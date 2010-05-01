#include "CPlayer.h"
#include "CSpell.h"
#include "CTerrainBase.h"
#include "CGame.h"
#include "CAnimation.h"
#include "Wrappers/CSGD_DirectInput.h"
#include "CAnimationWarehouse.h"

int CPlayer::PlayerCount = 0;

CPlayer::CPlayer()
{
	m_fFireTimer = 0.0; m_nHealth = 200; m_bIsJumping = false;
	currAnimation = 0;
	currDirec = RIGHT; m_nType = OBJ_PLAYER; m_fJumpTimer = 0.0f;

	SetPlayerID(++PlayerCount);

	if(GetPlayerID() == 1)
		m_SpellType = OBJ_ICE;
	else
		m_SpellType = OBJ_WIND;
}

void CPlayer::Update(float fElapsedTime)
{

	CCharacter::Update(fElapsedTime);

	CSGD_DirectInput * DI = (CSGD_DirectInput::GetInstance());
	
		if( ( GetPlayerID() == 1 && (DI->KeyDown( DIK_D )	  || DI->JoystickDPadDown(1)))	||
			( GetPlayerID() == 2 && (DI->KeyDown( DIK_RIGHT ) || DI->JoystickDPadDown(1, 1))) )
		{
			SetAnimation(0, 1);

			if( DI->KeyDown( DIK_W ) || DI->KeyDown( DIK_UP ) || DI->JoystickDPadDown(2, GetPlayerID() - 1) )
				currDirec = RIGHT_UP;

			else if( DI->KeyDown( DIK_S ) || DI->KeyDown( DIK_DOWN ) || DI->JoystickDPadDown(3, GetPlayerID() - 1))
				currDirec = RIGHT_DOWN;
			else
				currDirec = RIGHT;
		}

		else if( ( GetPlayerID() == 1 && (DI->KeyDown( DIK_A )	  || DI->JoystickDPadDown(0)))	||
				 ( GetPlayerID() == 2 && (DI->KeyDown( DIK_LEFT ) || DI->JoystickDPadDown(0, 1))) )
		{
			SetAnimation(0,1);


			if( DI->KeyDown( DIK_W ) || DI->KeyDown( DIK_UP ) || DI->JoystickDPadDown(2, GetPlayerID() - 1) )
				currDirec = LEFT_UP;
			else if( DI->KeyDown( DIK_S ) || DI->KeyDown( DIK_DOWN ) || DI->JoystickDPadDown(3, GetPlayerID() - 1))
				currDirec = LEFT_DOWN;
			else
				currDirec = LEFT;

		}
		else if( (GetPlayerID() == 1 && DI->KeyDown( DIK_S ))    || 
				 (GetPlayerID() == 2 && DI->KeyDown( DIK_DOWN )) || DI->JoystickDPadDown(3, GetPlayerID() - 1))
			currDirec = DOWN;
		else if( (GetPlayerID() == 1 && DI->KeyDown( DIK_W ))	 ||
				 (GetPlayerID() == 2 && DI->KeyDown( DIK_UP ))	 || DI->JoystickDPadDown(2, GetPlayerID() - 1) )
			currDirec = UP;
		else
		SetAnimation(0, 0);



	if(currDirec == RIGHT || currDirec== RIGHT_DOWN || currDirec == RIGHT_UP)
	{
		IsRotated = true;
	}
	else
	{
		IsRotated = false;
	}
	

	if(m_bIsJumping)
	{
		m_fJumpTimer += fElapsedTime;

		if(m_fJumpTimer <= .75)
			SetVelY(-100);
	}

	m_fFireTimer = m_fFireTimer + fElapsedTime;

	if(m_nHealth <=0)
	{
		animation = NULL;
	}
}

void CPlayer::Attack(int nTier)
{
	if(m_fFireTimer > .25f)
	{
		switch(m_SpellType)
		{
			case OBJ_EARTH:
				{
					if(m_fFireTimer < 1.5f)
						return;
				m_pSpells->CreateEarth(this, nTier);
				break;
				}
			case OBJ_FIRE:
				m_pSpells->CreateFire(this, nTier);
				break;
			case OBJ_ICE:
				m_pSpells->CreateIce(this, nTier);
				break;
			case OBJ_WIND:
				m_pSpells->CreateWind(this, nTier);
				break;
		}
		m_fFireTimer = 0.0f;
	}
}

void CPlayer::Jump()
{
	if(m_bIsJumping)
		return;

	m_bIsJumping = true;
	//m_fJumpTimer = 0.0f;
}

void CPlayer::HandleCollision(CBase * collidingObject)
{
	if( collidingObject->GetType() == OBJ_TERRA || (collidingObject->GetType() == OBJ_SPELL && ((CSpell*)collidingObject)->GetElement() == OBJ_EARTH))
	{
		if(collidingObject->GetType() == OBJ_TERRA)
		{

			int TerraType = ((CTerrainBase*)collidingObject)->GetTypeTerrain();

			if( TerraType == END_POINT)
				CGame::GetInstance()->ChangeState( CMenuState::GetInstance());

			if( TerraType == T_LAVA || TerraType == T_WATER )
				return;
		}

		if(collidingObject->GetType() == OBJ_SPELL && !((CSpell*)collidingObject)->PlayerShot())
		{
			TakeDamage(((CSpell*)collidingObject)->GetDamage());
		}

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
			{
				if(collidingObject->GetType() == OBJ_SPELL)
					return;
				SetPosY( GetPosY() + nRectHeight  );
				
			}
			else if(this->GetPosY() < collidingObject->GetPosY() )
			{
				
				m_bIsJumping = false;
				m_fJumpTimer = 0.0f;
				
				SetPosY( GetPosY() - nRectHeight );
			}

		}
	}

/*
	if(collidingObject->GetType() == OBJ_TERRA)
	{
		
	
		//If we've hit the tile from above or below.
		if(GetPosX() > collidingObject->GetPosX() && 
			GetPosX() < collidingObject->GetPosX() + collidingObject->GetWidth())
		{
			//From Above:
		if(GetPosY() < collidingObject->GetPosY())
			{
				SetVelY( 0.0f );
				SetPosY( collidingObject->GetPosY() - GetHeight() - 1);
				m_bIsJumping = false;
				m_fJumpTimer = 0.0f;
			}
			//From Below
			else
				SetPosY(collidingObject->GetPosY() + collidingObject->GetHeight() + 1);


			return;

		}
		//We've hit the tile from the left or right side.
		else
		{
			if(GetPosX() < collidingObject->GetPosX())
				SetPosX(collidingObject->GetPosX() - GetWidth());

			else if(GetPosY() + GetHeight() != collidingObject->GetPosY() + 1.25f)
				SetPosX(collidingObject->GetPosX() + collidingObject->GetWidth());
			return;
		}

		*/

		/*RECT collRect = {};

		IntersectRect(&collRect, &GetCollisionRect(CGame::GetInstance()->GetElapsedTime()), &collidingObject->GetCollisionRect(CGame::GetInstance()->GetElapsedTime()));*/

		//If the height is less than the top.
		//   __________
		//   |	      |
		//	 ----------
		/*if(collRect.bottom - collRect.top <= collRect.right - collRect.left)
		{
			if(collRect.top == collidingObject->GetPosY())
			{
				SetVelY( 0.0f );
				SetPosY( collidingObject->GetPosY() - GetHeight() - 1);
				m_bIsJumping = false;
				m_fJumpTimer = 0.0f;
			}
			else
				SetPosY(collidingObject->GetPosY() + collidingObject->GetHeight() + 1);

		}
		else
		{
			if(collRect.left == collidingObject->GetPosX())
				SetPosX(collidingObject->GetPosX() - GetWidth());
			else
				SetPosX(collidingObject->GetPosX() + collidingObject->GetHeight() + 1);

		}*/
	

	else if(collidingObject->GetType() == OBJ_SPELL && !((CSpell*)collidingObject)->PlayerShot())
	{
		m_nHealth -= ((CSpell*)collidingObject)->GetDamage();
	}
}