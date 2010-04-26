#include "CPlayer.h"
#include "CSpell.h"
#include "CGame.h"
#include "Wrappers/CSGD_DirectInput.h"

void CPlayer::Update(float fElapsedTime)
{

	CBase::Update(fElapsedTime);

	CSGD_DirectInput * DI = (CSGD_DirectInput::GetInstance());
	
	if( DI->KeyDown( DIK_D ) )
	{
		if( DI->KeyDown( DIK_W ) )
			currDirec = RIGHT_UP;
		else if( DI->KeyDown( DIK_S ) )
			currDirec = RIGHT_DOWN;
		else
			currDirec = RIGHT;
	}
	else if( DI->KeyDown( DIK_A ) )
	{
		if(DI->KeyDown( DIK_W ) )
			currDirec = LEFT_UP;
		else if( DI->KeyDown( DIK_S ) )
			currDirec = LEFT_DOWN;
		else
			currDirec = LEFT;

	}
	else if(DI->KeyDown(DIK_S))
		currDirec = DOWN;
	else if(DI->KeyDown(DIK_W))
		currDirec = UP;


	SetVelY(150);

	if(m_bIsJumping)
	{
		m_fJumpTimer += fElapsedTime;

		if(m_fJumpTimer <= .75)
			SetVelY(-100);
	}

	m_fFireTimer = m_fFireTimer + fElapsedTime;

}

void CPlayer::Attack(int nTier)
{
	if(m_fFireTimer > .25f)
	{
		switch(m_SpellType)
		{
			case OBJ_EARTH:
				m_pSpells->CreateEarth(this, nTier);
				break;
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
	if( collidingObject->GetType() == OBJ_TERRA )
	{
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