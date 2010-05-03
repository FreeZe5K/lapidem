#include "CPlayer.h"
#include "CSpell.h"
#include "CTerrainBase.h"
#include "CGame.h"
#include "CAnimation.h"
#include "Wrappers/CSGD_DirectInput.h"
#include "CAnimationWarehouse.h"
#include "Corona_ObjectManager.h"
#include "Corona_EventHandler.h"

int CPlayer::PlayerCount = 0;

CPlayer::CPlayer( )
{
	m_fFireTimer       = 0.0f; 
	m_nHealth          = 200; 
	m_fJumpTimer       = 0.0f;
	currAnimation      = 0;
	m_bIsJumping       = false;
	currDirec          = RIGHT; 
	m_nType            = OBJ_PLAYER; 

	SetPlayerID( ++PlayerCount );

	if( GetPlayerID( ) == 1 )
		m_SpellType = OBJ_ICE;
	else m_SpellType = OBJ_WIND;
}

void CPlayer::Update( float fElapsedTime )
{
	CCharacter::Update( fElapsedTime );

	CSGD_DirectInput * DI = ( CSGD_DirectInput::GetInstance( ) );

	if( ( GetPlayerID( ) == 1 && ( DI->KeyDown( DIK_D ) || 
		DI->JoystickDPadDown( 1 ) ) ) || ( GetPlayerID( ) == 2 && 
		( DI->KeyDown( DIK_RIGHT ) || DI->JoystickDPadDown( 1, 1 ) ) ) )
	{
		SetAnimation( 0, 1 );

		if( DI->KeyDown( DIK_W ) || DI->KeyDown( DIK_UP ) || 
			DI->JoystickDPadDown( 2, GetPlayerID( ) - 1 ) )
			currDirec = RIGHT_UP;

		else if( DI->KeyDown( DIK_S ) || DI->KeyDown( DIK_DOWN ) || 
			DI->JoystickDPadDown( 3, GetPlayerID( ) - 1 ) )
			currDirec = RIGHT_DOWN;
		else currDirec = RIGHT;
	}

	else if( ( GetPlayerID( ) == 1 && ( DI->KeyDown( DIK_A ) || 
		DI->JoystickDPadDown( 0 ) ) ) || ( GetPlayerID( ) == 2 && 
		( DI->KeyDown( DIK_LEFT ) || DI->JoystickDPadDown( 0, 1 ) ) ) )
	{
		SetAnimation( 0, 1 );

		if( DI->KeyDown( DIK_W ) || DI->KeyDown( DIK_UP ) || 
			DI->JoystickDPadDown( 2, GetPlayerID( ) - 1 ) )
			currDirec = LEFT_UP;
		else if( DI->KeyDown( DIK_S ) || DI->KeyDown( DIK_DOWN ) || 
			DI->JoystickDPadDown( 3, GetPlayerID( ) - 1 ) )
			currDirec = LEFT_DOWN;
		else currDirec = LEFT;
	}
	else if( ( GetPlayerID( ) == 1 && DI->KeyDown( DIK_S ) ) || 
		( GetPlayerID( ) == 2 && DI->KeyDown( DIK_DOWN )) || 
		DI->JoystickDPadDown( 3, GetPlayerID( ) - 1 ) )
		currDirec = DOWN;
	else if( ( GetPlayerID( ) == 1 && DI->KeyDown( DIK_W ))	||
		( GetPlayerID( ) == 2 && DI->KeyDown( DIK_UP ) ) || 
		DI->JoystickDPadDown( 2, GetPlayerID( ) - 1 ) )
		currDirec = UP;
	else SetAnimation( 0, 0 );

	if( currDirec == RIGHT || currDirec == RIGHT_DOWN || currDirec == RIGHT_UP )
		IsRotated = true;
	else IsRotated = false;

	if( m_bIsJumping )
	{
		m_fJumpTimer = m_fJumpTimer + fElapsedTime;

		if( m_fJumpTimer <= 0.25f )
		{
			SetVelY( -200 );
			SetAnimation( 0, 0 );
		}
		else if( m_fJumpTimer <= 0.5f )
		{
			SetVelY( -150 );
			SetAnimation( 0, 0 );
		}
		else if( m_fJumpTimer < 0.75f )
		{
			SetVelY( -100 );
			SetAnimation( 0, 0 );
		}
		else if( m_fJumpTimer >= .75 && m_fJumpTimer <= 0.8f )
		{
			SetVelY( 0.0f );
			SetAnimation( 0, 0 );
		}
		else if(m_fJumpTimer > 0.8f && m_fJumpTimer <= 1 )
		{
			SetVelY( 100 );
			SetAnimation( 0, 0 );
		}
		else SetAnimation( 0, 0 );
	}

	m_fFireTimer = m_fFireTimer + fElapsedTime;

	if( m_nHealth <= 0 )
		animation = NULL;

	if(	GetPlayerID( ) == 2 )
	{
		if( !Corona_ObjectManager::GetInstance( )->IsOnScreen( this ) )
			Corona_EventHandler::GetInstance( )->SendEvent( "P2 OFFSCREEN" );
		else
		{
			RECT rCam, r;
			CCamera* c( CCamera::GetCamera( ) );
			rCam.left    = LONG( c->GetXOffset( ) );
			rCam.top     = LONG( c->GetYOffset( ) );
			rCam.right   = LONG( c->GetWidth( ) );
			rCam.bottom  = LONG( c->GetHeight( ) );

			RECT rPlayer( GetCollisionRect( fElapsedTime ) );
			IntersectRect( &r, &rCam, &rPlayer );

			if( r.left !=rPlayer.left || r.right != rPlayer.right )
			{
				if( r.left > rPlayer.left )
					SetPosX( GetPosX( ) - ( rPlayer.left - r.left ) );
				if( r.right < rPlayer.right )
					SetPosX( GetPosX( ) + ( r.right - rPlayer.right ) );
			}
		}
	}
}

void CPlayer::Attack( int nTier )
{
	if( m_fFireTimer > 0.25f )
	{
		switch( m_SpellType )
		{
		case OBJ_EARTH:
			{
				if( m_fFireTimer < 1.5f )
					return;
				m_pSpells->CreateEarth( this, nTier );
				break;
			}

		case OBJ_FIRE:
			m_pSpells->CreateFire( this, nTier );
			break;

		case OBJ_ICE:
			m_pSpells->CreateIce( this, nTier );
			break;

		case OBJ_WIND:
			m_pSpells->CreateWind( this, nTier );
			break;
		}
		m_fFireTimer = 0.0f;
	}
}

void CPlayer::Jump( )
{
	if( m_bIsJumping )
		return;

	m_bIsJumping = true;
	//m_fJumpTimer = 0.0f;
}

void CPlayer::HandleCollision( CBase * collidingObject )
{
	if( collidingObject->GetType( ) == OBJ_TERRA || ( collidingObject->GetType( ) == 
		OBJ_SPELL && ( ( CSpell* )collidingObject )->GetElement( ) == OBJ_EARTH ) )
	{
		if( collidingObject->GetType( ) == OBJ_TERRA )
		{
			int TerraType( ( ( CTerrainBase* )collidingObject )->GetTypeTerrain( ) );

			if( TerraType == END_POINT )
				CGame::GetInstance( )->ChangeState( CMenuState::GetInstance( ) );

			if( TerraType == T_LAVA || TerraType == T_WATER )
				return;
		}

		if( collidingObject->GetType( ) == OBJ_SPELL && 
			!( ( CSpell* )collidingObject )->PlayerShot( ) )
			TakeDamage( ( ( CSpell* )collidingObject )->GetDamage( ) );

		RECT r;
		IntersectRect( &r, & this->GetCollisionRect( 0 ), 
			&collidingObject->GetCollisionRect( 0 ) );

		int nRectWidth( r.right -r.left );
		int nRectHeight( r.bottom - r.top );

		if( nRectHeight > nRectWidth )
		{
			if( this->GetPosX( ) > collidingObject->GetPosX( ) )
				SetPosX( GetPosX( ) + nRectWidth );
			else if ( this->GetPosX( ) < collidingObject->GetPosX( ) )
				SetPosX( GetPosX( ) - nRectWidth );
		}
		else if( nRectHeight < nRectWidth ) 
		{
			if( this->GetPosY( ) > collidingObject->GetPosY( ) )
			{
				if( collidingObject->GetType( ) == OBJ_SPELL )
					return;

				SetPosY( GetPosY( ) + nRectHeight );
			}
			else if( this->GetPosY( ) < collidingObject->GetPosY( ) )
			{
				m_bIsJumping = false;
				m_fJumpTimer = 0.0f;

				SetPosY( GetPosY( ) - nRectHeight );
			}
		}
	}
	else if( collidingObject->GetType( ) == OBJ_SPELL && !( ( CSpell* )collidingObject )->PlayerShot( ) )
		m_nHealth = m_nHealth - ( ( CSpell* )collidingObject )->GetDamage( );
}