//////////////////////////////////////////////////////////////////////////
//  File Name   :   "CWind.h"
//
//  Author Name :   Jonathan Caro
//
//  Purpose     :	Handles the wind objects created by the player and
//					enemies. Handles all 3 tiers of spells.
//////////////////////////////////////////////////////////////////////////
#include "CWind.h"
#include "Wrappers/CSGD_TextureManager.h"
#include "CCamera.h"
#include "CSpellFactory.h"
#include "CPlayer.h"
#include "CEnemy.h"
#include "Corona_ObjectManager.h"
#include "Lapidem_Math.h"


CWind::CWind( ) : CSpell( )
{
	//SetImage( CSGD_TextureManager::GetInstance( )->LoadTexture
		//( "resource/graphics/wind.bmp", D3DCOLOR_XRGB( 0, 0, 0 ) ) );
	m_fShotTimer = 0.5f;
}

CWind::~CWind( )
{	

}

void CWind::Update( float fElapsedTime )
{
	SetLifespan( GetLifespan( ) - fElapsedTime );

	if( GetLifespan( ) < 0 )
		SetActive( false );

	switch( GetTier( ) )
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

void CWind::UpdateTier1( float fElapsedTime )
{
	SetPosX( GetPosX( ) + GetVelX( ) * fElapsedTime );
	SetPosY( GetPosY( ) + GetVelY( ) * fElapsedTime );	
}

void CWind::UpdateTier2( float fElapsedTime )
{
	if(m_pCaster->GetReticle() != NULL)
	{
		tVector2D newpos, currpos;
		currpos._x = GetPosX( );
		currpos._y = GetPosY( );
		newpos._x = m_pCaster->GetPosX();
		newpos._y = m_pCaster->GetPosY();

		float dot = (newpos._x * currpos._x)+(currpos._y * newpos._y);
		float length = sqrt((newpos._x * newpos._x) + (newpos._y * newpos._y))* sqrt( ( currpos._x * currpos._x ) + ( currpos._y * currpos._y ) );
		float angle = float( acos( dot / length ) + PI / 4.0f );
		angle = angle * 180.0f / D3DX_PI;
		if(angle > 45.0f && angle < 135.0f)
		{
			m_pCaster->SetVelY(-100);
		}
	}
	else if(m_pCaster->GetDirection() == DOWN || m_pCaster->GetDirection() == RIGHT_DOWN ||m_pCaster->GetDirection() == LEFT_DOWN)
	{
		m_pCaster->SetVelY(-100);
	}
	CSpell::UpdateTier2(fElapsedTime);
}

void WindTier3(CBase* pEnemy, CBase* pSpell)
{	
	float posx, posy;
	posx = pSpell->GetPosX( );
	posy = pSpell->GetPosY( );

	posx = posx - pEnemy->GetPosX( );
	posy = posy - pEnemy->GetPosY( );

	posx = posx * posx;
	posy = posy * posy;

	posx = posx + posy;

	//posx = sqrt(posx);

	if(posx < (150 *150) )
	{
		CWind* newwind = new CWind;
		if(pEnemy->GetPosX() < pSpell->GetPosX())
		{
			newwind->SetPosX(pEnemy->GetPosX()+1);
		}
		else
		{
			newwind->SetPosX(pEnemy->GetPosX());
		}
		newwind->SetVelX(pEnemy->GetVelX());
		newwind->SetPosY(pEnemy->GetPosY());
		newwind->SetActive(true);
		newwind->SetElement(OBJ_WIND);
		newwind->SetDamage(2);
		newwind->SetPushBack(10.0f);
		newwind->SetLifespan(0.2f);
		newwind->SetTier(1);
		newwind->SetWidth(32);
		newwind->ShotBy(true);
		newwind->SetHeight(16);
		Corona_ObjectManager::GetInstance()->AddObject(newwind);
		newwind->Release();
	}
}

void CWind::UpdateTier3( float fElapsedTime )
{
	SetPosX(m_pCaster->GetPosX());
	SetPosY(m_pCaster->GetPosY());
	m_fShotTimer -= fElapsedTime;
	if(m_fShotTimer < 0.0f)
	{
		Corona_ObjectManager::GetInstance()->AuxFunction(&WindTier3,OBJ_ENEMY, false,this);	
		m_fShotTimer = 0.5f;
	}
}

void CWind::Render( )
{
	switch( GetTier( ) )
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

void CWind::RenderTier1( )
{
	if( GetImage( ) != -1 )
	{
		CSGD_TextureManager::GetInstance( )->Draw( GetImage( ), 
			int( GetPosX( ) - CCamera::GetCamera( )->GetXOffset( ) ), 
			int( GetPosY( )- CCamera::GetCamera( )->GetYOffset( ) ) );
	}
}

void CWind::RenderTier2( )
{ /* NOTHING HERE YET */ }

void CWind::RenderTier3( )
{ /* NOTHING HERE YET */ }

void CWind::HandleCollision( CBase* pObject )
{
	CSpell::HandleCollision(pObject);

	if( 1 == GetTier( ) )
	{
		if( pObject->GetType( ) == OBJ_TERRA )
		{
			SetActive( false );
			CSpellFactory::GetInstance()->AddWindXP(2);
		}
		else if( pObject->GetType( ) == OBJ_PLAYER && !PlayerShot( ) )
		{
			SetActive( false );
			( ( CPlayer* )pObject )->TakeDamage( GetDamage( ) );
		}
		else if( pObject->GetType( ) == OBJ_ENEMY )
		{
			SetActive( false );
			if(GetPosX() > pObject->GetPosX())
			{
				pObject->SetVelX(-150);
			}
			else
			{
				pObject->SetVelX(150);
			}
			((CEnemy*)pObject)->SetKnockBack(GetPushBack());
			CSpellFactory::GetInstance()->AddWindXP(2);
		}
		else if( pObject->GetType( ) == OBJ_SPELL && ( ( CSpell* )pObject )->GetElement( ) != GetElement( ) 
			&& ( ( CSpell* )pObject )->GetTier( ) != 3)
				SetActive( false );
	}
	else if( 2 == GetTier( ) )
	{ /* do stuff... like destroy... EVERYTHING */ }
	else if( 3 == GetTier( ) )
	{ /* holy crap everything go splode */ }
}