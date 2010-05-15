#include "CCharacter.h"
#include "CAnimationWarehouse.h"
#include "CAnimation.h"
#include "CCamera.h"
#include "CSpell.h"
#include "Wrappers/CSGD_TextureManager.h"
#include <windows.h>

CCharacter::CCharacter( )
{
	m_pSpells = CSpellFactory::GetInstance( );
	animation = NULL;
}

void CCharacter::Render( )
{
	if( animation )
	{
		if( animation->GetImageID( ) != -1 && !IsRotated )
			CSGD_TextureManager::GetInstance( )->Draw( animation->GetImageID( ), 
			int( GetPosX( ) - CCamera::GetCamera( )->GetXOffset( ) ),
			int( GetPosY( ) - CCamera::GetCamera( )->GetYOffset( ) ),
			1.0f, 1.0f, &animation->GetFrames( )->DrawRect );
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
		SetWidth( animation->GetFrames( )->DrawRect.right - animation->GetFrames( )->DrawRect.left );
		SetHeight( animation->GetFrames( )->DrawRect.bottom - animation->GetFrames( )->DrawRect.top );
	}
	SetVelY( 200 );
}

void CCharacter::SetAnimation( int object, int animation, int frame )
{
	this->animation = ( CAnimationWarehouse::GetInstance
		( )->GetAnimation(object,animation ) );
	currAnimation = 0;
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