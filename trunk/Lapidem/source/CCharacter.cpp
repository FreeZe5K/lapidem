#include "CCharacter.h"
#include "stdheaders.h"
#include "CAnimationWarehouse.h"
#include "CAnimation.h"
#include "CSpell.h"


#define Fall_Rate 200

CCharacter::CCharacter( )
{
	m_pSpells = CSpellFactory::GetInstance( );
	animation = NULL;

	IsRotated = false;
	m_nHealth = -1;
	currAnimation = -1;
	m_SpellType = OBJ_FIRE;
	currDirec = RIGHT;
}

void CCharacter::Render( )
{
	if( animation )
	{
		if( animation->GetImageID( ) != -1 && !IsRotated )
			CSGD_TextureManager::GetInstance( )->Draw( animation->GetImageID( ), 
			int( GetPosX( ) - CCamera::GetCamera( )->GetXOffset( ) ),
			int( GetPosY( ) - CCamera::GetCamera( )->GetYOffset( ) ),
			1.0f, 1.0f, &animation->GetFrames( )->DrawRect);
		else if( animation->GetImageID( ) != -1 && IsRotated )
		{
			CSGD_TextureManager::GetInstance( )->Draw( animation->GetImageID( ), 
				int( GetPosX( ) - CCamera::GetCamera( )->GetXOffset( ) + GetWidth( ) ), 
				int( GetPosY( ) - CCamera::GetCamera( )->GetYOffset( ) ),
				-1.0f, 1.0f, &animation->GetFrames( )->DrawRect );
		}
	} else CBase::Render( );
}

void CCharacter::Update( float fElapsedTime )
{
	CBase::Update( fElapsedTime );
	if( animation )
	{
		animation->Update( fElapsedTime );
	}

	if(GetVelY() > 3 && GetVelY() < 50)
		SetVelY(GetVelY() + Fall_Rate * 2 * fElapsedTime);
	SetVelY( GetVelY() + Fall_Rate * fElapsedTime );
}

void CCharacter::SetAnimation( int object, int animation, int frame )
{
	this->animation = ( CAnimationWarehouse::GetInstance
		( )->GetAnimation(object,animation ) );
	currAnimation = animation;
}


void CCharacter::SetEleType( EleType newType )
{
	if( GetType() == OBJ_ENEMY)
	{
	if( newType > OBJ_NONE ) 
		newType = OBJ_FIRE; 
	else if( newType < OBJ_FIRE ) 
		newType = OBJ_NONE; 
	}
	else if (GetType() == OBJ_PLAYER)
	{
		if( newType > OBJ_EARTH )
			newType = OBJ_FIRE;
		else if( newType < OBJ_FIRE)
			newType = OBJ_EARTH;
	}
	m_SpellType = newType;

}
int CCharacter::TakeDamage(int nDamage)
{
	int nHealth = m_nHealth;

	m_nHealth -= nDamage;

	return nHealth - m_nHealth;
}