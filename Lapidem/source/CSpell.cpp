//////////////////////////////////////////////////////////////////////////
//  File Name   :   "CSpell.cpp"
//
//  Author Name :   Jonathan Caro
//
//  Purpose     :   Provides the functionality for the spells to inherit 
//////////////////////////////////////////////////////////////////////////
#include "Wrappers/CSGD_TextureManager.h"
#include "CGameplayState.h"
#include "Corona_ObjectManager.h" 
#include "CEmitter.h"
#include "CParticleManager.h"
#include "CSpell.h"
#include "CSpellFactory.h"

CSpell::CSpell( ) : CBase( )
{
	SetType(OBJ_SPELL);

	// REMOVE ME LATER
	
}

CSpell::~CSpell()
{ 

}

void CSpell::UpdateTier1( float fElapsedTime )
{		
	// - - - - - - - -
	// REPLACE (AND FIX)
	// - - - - - - - - - - - - - - 
	/*if( IsActive( ) )
	{
		CGameplayState::GetInstance( )->GetFire( )->SetPosX( GetPosX( ) );
		CGameplayState::GetInstance( )->GetFire( )->SetPosY( GetPosY( ) );
	} */
	//m_pEmitter->UpdateParticlePos( int( GetPosX( ) ), int( GetPosY( ) ) );

	SetPosX( GetPosX( ) + GetVelX( ) * fElapsedTime );
	SetPosY( GetPosY( ) + GetVelY( ) * fElapsedTime );
}
void CSpell::UpdateTier2( float fElapsedTime )
{
	SetPosX( GetPosX( ) + GetVelX( ) * fElapsedTime );
	SetPosY( GetPosY( ) + GetVelY( ) * fElapsedTime );
}

void CSpell::UpdateTier3( float fElapsedTime )
{
	SetPosX( GetPosX( ) + GetVelX( ) * fElapsedTime );
	SetPosY( GetPosY( ) + GetVelY( ) * fElapsedTime );
}
void CSpell::RenderTier1( )
{
	if( GetImage( ) != -1 )
		CSGD_TextureManager::GetInstance( )->Draw
		( GetImage( ), int( GetPosX( ) ), int( GetPosY( ) ) );
}
void CSpell::RenderTier2( )
{
	if( GetImage( ) != -1 )
		CSGD_TextureManager::GetInstance( )->Draw
		( GetImage( ), int( GetPosX( ) ), int( GetPosY( ) ) );
}
void CSpell::RenderTier3( )
{
	if( GetImage( ) != -1 )
		CSGD_TextureManager::GetInstance( )->Draw
		( GetImage( ), int( GetPosX( ) ), int( GetPosY( ) ) );
}
void CSpell::Update( float fElapsedTime )
{
	SetLifespan( GetLifespan( ) - fElapsedTime);



	if( GetLifespan( ) <= 0 )
		SetActive( false );

	switch( m_nTier )
	{
	case 1:
		{
			UpdateTier1( fElapsedTime );
			break;
		}
	case 2:
		{
			UpdateTier2( fElapsedTime );
			break;
		}
	case 3:
		{
			UpdateTier3( fElapsedTime );
			break;
		}
	}
}

void CSpell::HandleCollision(float fElapsedTime, CBase* pObject )
{
	////////////////////////////////////////////////////////////////
	//TODO: Carona_ObjectManager->GetInstance()->RemoveObject(this);
	////////////////////////////////////////////////////////////////
	if(pObject->GetType() == OBJ_SPELL)
	{
		if(this->PlayerShot() == ((CSpell*)pObject)->PlayerShot())
		{
			if(this->GetTier() == 1 && ((CSpell*)pObject)->GetTier() == 1)
			{
				CreateCombinedSpell(this, (CSpell*)pObject);
			}	// if is tier one
		}
		else
		{
			if(GetTier() ==3)
			{
				return;
			}
			if(CancelSpell((CSpell*)pObject))
				this->SetActive(false);
			CreateCollisionEffect(this->GetElement(), ((CSpell*)pObject)->GetElement());
		}	// if player shot
	}
}

void CSpell::Render( )
{
	switch( m_nTier )
	{
	case 1:
		{
			RenderTier1( );
			break;
		}
	case 2:
		{
			RenderTier2( );
			break;
		}
	case 3:
		{
			RenderTier3( );
			break;
		}
	}
}

void CSpell::CreateCollisionEffect(int nFirst, int nSecond)
{
	// Create Effect on Inpact
	switch(nFirst)
	{
	case OBJ_FIRE:
		if(nSecond == OBJ_FIRE)
			{
		} // Create Fire blast effect
		else if(nSecond == OBJ_ICE)
			{
		} // create steam
		else if(nSecond == OBJ_WIND)
			{
		} // create scatter
		else if(nSecond == OBJ_EARTH)
			{
		} // fire scatters
		break;
	case OBJ_ICE:
		if(nSecond == OBJ_ICE)
		{
		}// create pieces of ice
		else if(nSecond == OBJ_WIND)
		{
		}// create ice dust...
		else if(nSecond == OBJ_EARTH)
		{
		}
		break;
	case OBJ_WIND:
		if(nSecond == OBJ_WIND)
			{
		} // create whirlwind
		else if(nSecond == OBJ_EARTH)
			{
		} // rock pieces
		break;
	case OBJ_EARTH:
		if(nSecond == OBJ_EARTH)
			{
		} // creates dust
		break;
	}
}

bool CSpell::CancelSpell(CSpell* pOther)
{
	int nMyType = this->GetElement();
	int nOtherType = pOther->GetElement();

	switch(nMyType)
	{
	case OBJ_FIRE:
		if(nOtherType == OBJ_ICE)
			return false;
		break;
	case OBJ_ICE:
		if(nOtherType == OBJ_WIND)
			return false;
		break;
	case OBJ_WIND:
		if(nOtherType == OBJ_EARTH)
			return false;
		break;
	case OBJ_EARTH:
		if(nOtherType == OBJ_FIRE)
			return false;
		break;
	}

	return true;
}

void CSpell::CreateCombinedSpell(CSpell* pFirst, CSpell* pSecond)
{
	int nFirst = pFirst->GetElement();
	int nSecond = pSecond->GetElement();
	switch(nFirst)
	{
	case OBJ_FIRE:
		if(nSecond == OBJ_WIND)
			CSpellFactory::GetInstance()->CreateGiantFireBall(pFirst, pSecond); // create scatter
		else if(nSecond == OBJ_EARTH)
			CSpellFactory::GetInstance()->CreateGrenade(pFirst, pSecond); // fire scatters
		break;
	case OBJ_ICE:
		if(nSecond == OBJ_WIND)
			CSpellFactory::GetInstance()->CreateSpear(pFirst, pSecond); // create ice dust...
		else if(nSecond == OBJ_EARTH)
			CSpellFactory::GetInstance()->CreateIceCube(pFirst, pSecond); // creates icy rock
		break;
	}
}