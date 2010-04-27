#include "CCharacter.h"
#include "CAnimationWarehouse.h"
#include "CAnimation.h"
#include "CCamera.h"
#include "Wrappers/CSGD_TextureManager.h"
#include <windows.h>

CCharacter::CCharacter()
{
	m_pSpells = CSpellFactory::GetInstance();
	animation = CAnimationWarehouse::GetInstance()->GetAnimation(0,0);
}

void CCharacter::Render()
{
	if( animation->GetImageID() != -1 && !IsRotated)
		CSGD_TextureManager::GetInstance( )->Draw( animation->GetImageID( ), ( int )(GetPosX( ) - CCamera::GetCamera()->GetXOffset()), ( int )(GetPosY( ) - CCamera::GetCamera()->GetYOffset()),1.0f,1.0f,&animation->GetFrames()->DrawRect);
	else if(IsRotated)
	{
		CSGD_TextureManager::GetInstance( )->Draw( animation->GetImageID( ), ( int )(GetPosX( ) - CCamera::GetCamera()->GetXOffset() + (GetWidth() >>1)), ( int )(GetPosY( ) - CCamera::GetCamera()->GetYOffset()),-1.0f,1.0f,&animation->GetFrames()->DrawRect);
	}
	
}