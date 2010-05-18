#include "stdheaders.h"
#include "CPlayer.h"
#include "CSpell.h"
#include "CTerrainBase.h"
#include "CTAirCurrent.h"
#include "CPickups.h"
#include "CAnimation.h"
#include "CAnimationWarehouse.h"


int CPlayer::PlayerCount = 0;

#define Jump_Value 175
#define X_Diag 150
#define Y_Diag 175

CPlayer::CPlayer( )
{
	m_fFireTimer       = 0.0f;
	m_fDrownTimer	   = 0.0f;
	m_nHealth          = 200; 
	currAnimation      = 0;
	currDirec          = RIGHT; 
	m_nType            = OBJ_PLAYER; 
	m_nFireEnergy	   = 0;
	m_nEarthEnergy	   = 0;
	m_nWindEnergy	   = 0;
	m_nWaterEnergy	   = 0;
	m_nScore		   = 0;
	m_nTierThree	   = 0;
	m_bIsJumping       = false;
	Tossed			   = false;
	m_bIsDrowning	   = false;
	m_bShielded		   = false;
	m_bIsTouching	   = false;
	m_bFainted			= false;
	m_fShieldTimer = 30.0f;
	RetPosX = 0;
	RetPosY = 0;

	m_pReticle		   = NULL;

	SetPlayerID( ++PlayerCount );

	if( GetPlayerID( ) == 1 )
		m_SpellType = OBJ_ICE;
	else m_SpellType = OBJ_WIND;

	Corona_EventHandler::GetInstance()->RegisterClient(this, "TileDestroyed");
	Corona_EventHandler::GetInstance()->RegisterClient(this, "EnemyDied");
}

CPlayer::~CPlayer( )
{
	--PlayerCount; 

	if( m_pReticle ) ToggleReticle( );

	Corona_EventHandler::GetInstance( )->UnregisterClient( "TileDestroyed", this );
	Corona_EventHandler::GetInstance( )->UnregisterClient( "EnemyDied", this );
}


void CPlayer::Update( float fElapsedTime )
{
	if( GetHealth() < 0 )
		SetHealth( 0 );


	if( GetFainted() )
	{
		animation = 0;

		if(m_pReticle)
		{
		Corona_ObjectManager::GetInstance()->RemoveObject(m_pReticle);
		m_pReticle->Release();
		m_pReticle = NULL;
		}	
		return;

	}

	m_fShieldTimer -= fElapsedTime;
	if(m_fShieldTimer < 0)
	{
		m_bShielded = false;
		m_fShieldTimer = 30.0f;
	}			

	if(Tossed)
	{

		if(GetDirection() == RIGHT_UP || GetDirection() == RIGHT || GetDirection() == RIGHT_DOWN)
		{
			SetVelX(GetVelX() - fElapsedTime * 40);

			if(GetVelX() < 100)
				Tossed = false;
		}
		else if (GetDirection() == LEFT_UP || GetDirection() == LEFT || GetDirection() == LEFT_DOWN)
		{
			SetVelX(GetVelX() + fElapsedTime  * 40);
			if(GetVelX() > -100)
				Tossed = false;
		}

		if(GetDirection() == LEFT_UP || GetDirection() == UP || GetDirection() == RIGHT_UP)
		{
			SetVelY(GetVelY() + fElapsedTime * 50 );

			if(GetDirection() == UP && GetVelY() >= 0)
				Tossed = false;
		}
		else if(GetDirection() == LEFT_DOWN || GetDirection() == DOWN || GetDirection() == RIGHT_DOWN)
		{
			SetVelY(GetVelY() - fElapsedTime * 50);

			if(GetDirection() == DOWN && GetVelY() <= 0)
				Tossed = false;
		}

		CBase::Update( fElapsedTime);
		return;
	}
	else
		CCharacter::Update( fElapsedTime );


	CSGD_DirectInput * DI = ( CSGD_DirectInput::GetInstance( ) );

	if( DI->JoystickButtonPressed(11, GetPlayerID() - 1) )
		ToggleReticle();

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

	if(DI->JoystickGetLStickXNormalized( GetPlayerID() - 1) < 0)
	{
		SetAnimation(0, 1);

		if(DI->JoystickGetLStickYNormalized(GetPlayerID() - 1) < 0)
			currDirec = LEFT_UP;
		else if(DI->JoystickGetLStickYNormalized(GetPlayerID() - 1) > 0 )
			currDirec = LEFT_DOWN;
		else
			currDirec = LEFT;
	}
	else if (DI->JoystickGetLStickXNormalized( GetPlayerID() - 1 ) > 0 )
	{
		SetAnimation(0 , 1);

		if(DI->JoystickGetLStickYNormalized(GetPlayerID() - 1) < 0)
			currDirec = RIGHT_UP;
		else if(DI->JoystickGetLStickYNormalized(GetPlayerID() - 1) > 0 )
			currDirec = RIGHT_DOWN;
		else
			currDirec = RIGHT;
	}

	if( currDirec == RIGHT || currDirec == RIGHT_DOWN || currDirec == RIGHT_UP )
		IsRotated = true;
	else IsRotated = false;

	if(m_pReticle)
	{
		float SpeedX = DI->JoystickGetRStickXNormalized(GetPlayerID() - 1);
		float SpeedY = DI->JoystickGetRStickYNormalized(GetPlayerID() - 1);

		m_pReticle->SetVelX(500 * SpeedX);
		m_pReticle->SetVelY(500 * SpeedY);

		m_pReticle->ClampToScreen();

		if(m_pReticle->GetVelX() != 0)
			RetPosX = m_pReticle->GetPosX() - CCamera::GetCamera()->GetXOffset();
		else
			m_pReticle->SetPosX(RetPosX + CCamera::GetCamera()->GetXOffset());

		if(m_pReticle->GetVelY() == 0)
			m_pReticle->SetPosY(RetPosY + CCamera::GetCamera()->GetYOffset());
		else
			RetPosY = m_pReticle->GetPosY() - CCamera::GetCamera()->GetYOffset();

	}
	else
	{
		RetPosY = GetPosY() - 32 - CCamera::GetCamera()->GetYOffset();
		RetPosX = GetPosX() - CCamera::GetCamera()->GetXOffset();
	}



	//******************************************************************************
	//******************************************************************************
	//******************************************************************************

	if(m_bIsJumping)
		SetAnimation(0,0);

	if(!m_bIsTouching)
		m_bIsDrowning = false;


	m_bIsTouching = false;
	m_fDrownTimer -=fElapsedTime;

	if(m_bIsDrowning)
	{
		if(!m_bIsJumping)
			SetVelY(25);
		else SetVelY(-150);
	}

	m_fFireTimer = m_fFireTimer + fElapsedTime;

	
	//******************************************************************************
	//******************************************************************************
	//******************************************************************************
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
	if(nTier == 3)
		if(this->GetT3Count() <= 0)
			return;
		else
			this->m_nTierThree -= 1;

	if( m_fFireTimer > 0.25f )
	{
		switch( m_SpellType )
		{
		case OBJ_EARTH:
			if( m_fFireTimer < .75f )
				return;

			if(nTier == 2)
			{
				if(m_nEarthEnergy < 5)
					return;
				else m_nEarthEnergy -= 5;
			}
			m_pSpells->CreateEarth( this, nTier );
			CSGD_WaveManager::GetInstance( )->Play
				( CGame::GetInstance( )->GetShotFiredSound( ) );
			break;
		case OBJ_FIRE:
			if(nTier == 2)
			{
				if(m_nFireEnergy < 2)
					return;
				else m_nFireEnergy -= 2;
			}
			m_pSpells->CreateFire( this, nTier );
			CSGD_WaveManager::GetInstance( )->Play
				( CGame::GetInstance( )->GetShotFiredSound( ) );
			break;
		case OBJ_ICE:
			if(nTier == 2)
			{
				if(m_nWaterEnergy < 2)
					return;
				else m_nWaterEnergy -= 2;
			}
			m_pSpells->CreateIce( this, nTier );
			CSGD_WaveManager::GetInstance( )->Play
				( CGame::GetInstance( )->GetShotFiredSound( ) );
			break;

		case OBJ_WIND:
			if(nTier == 2)
			{
				if(m_nWindEnergy < 2) return;
				else m_nWindEnergy = m_nWindEnergy - 2;
			}
			m_pSpells->CreateWind( this, nTier );
			CSGD_WaveManager::GetInstance( )->Play
				( CGame::GetInstance( )->GetShotFiredSound( ) );

			break;
		}
		m_fFireTimer = 0.0f;
	}
}

void CPlayer::Jump( )
{
	if( m_bIsJumping )
		return;

	SetVelY( -Jump_Value );
	m_bIsJumping = true;
}

void CPlayer::HandleCollision(float fElapsedTime, CBase * collidingObject )
{
	if( GetFainted()  || collidingObject->GetType() < 0)
	{
		return;
	}
		
	if( collidingObject->GetType( ) == OBJ_TERRA || ( collidingObject->GetType( ) == 
		OBJ_SPELL && ( ( CSpell* )collidingObject )->GetElement( ) == OBJ_EARTH ) )
	{
		if( collidingObject->GetType( ) == OBJ_TERRA )
		{
			int TerraType( ( ( CTerrainBase* )collidingObject )->GetTypeTerrain( ) );

			if( TerraType == T_LAVA || TerraType == T_WATER)
			{
				m_bIsDrowning = true;
				m_bIsTouching = true;

				if(m_fDrownTimer <= 0)
				{
					TakeDamage(5);
					m_fDrownTimer = 1.f;
				}
				return;
			}
			else m_bIsDrowning = false;
		}

		if( collidingObject->GetType( ) == OBJ_SPELL)
		{
			if(((CSpell*)collidingObject)->GetTier() == 3)
			{ return; }
			if(!((CSpell*)collidingObject )->PlayerShot())
			{ 
				TakeDamage( ( ( CSpell* )collidingObject )->GetDamage( ) ); 

				CSGD_WaveManager::GetInstance( )->Play
				( CGame::GetInstance( )->GetPlayerHitSound( ) );
			}
		}

		RECT r;
		IntersectRect( &r, & this->GetCollisionRect( fElapsedTime ), 
			&collidingObject->GetCollisionRect( fElapsedTime ) );

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
				SetVelY(0.0);
				SetPosY( GetPosY( ) - nRectHeight );
			}
		}
	}
	else if( collidingObject->GetType( ) == OBJ_SPELL && !( ( CSpell* )collidingObject )->PlayerShot( ) )
	{
		CSGD_WaveManager::GetInstance( )->Play
				( CGame::GetInstance( )->GetPlayerHitSound( ) );

		TakeDamage(( ( CSpell* )collidingObject )->GetDamage( ) );
	}
	else if( collidingObject->GetType( ) == OBJ_EVENT )
	{
		int TerraType( ( ( CTerrainBase* )collidingObject )->GetTypeTerrain( ) );

		if( TerraType == END_POINT )
			CGameplayState::GetInstance( )->SetPlayerReachedEnd( true );
		else if( TerraType == AIR_CURRENT )
		{
			Tossed = true;
			switch(((CTAirCurrent*)collidingObject)->GetDirection())
			{
			case RIGHT_UP:
				SetVelX(X_Diag);
				SetVelY(-Y_Diag);
				SetDirection(RIGHT_UP);
				break;
			case LEFT_UP:
				SetVelX(-X_Diag);
				SetVelY(-Y_Diag);
				SetDirection(LEFT_UP);
				break;
			case RIGHT:
				SetVelX(250);
				SetDirection(RIGHT);
				break;
			case LEFT:
				SetVelX(-250);
				SetDirection(LEFT);
				break;
			case UP:
				SetVelY(-250);
				SetVelX(0.0f);
				SetDirection(UP);
				break;
			case DOWN:
				SetVelY(125);
				SetVelX(0.0f);
				SetDirection(DOWN);
				break;
			case LEFT_DOWN:
				SetVelX(-X_Diag);
				SetVelY(Y_Diag);
				SetDirection(LEFT_DOWN);
				break;
			case RIGHT_DOWN:
				SetVelX(X_Diag);
				SetVelY(Y_Diag);
				SetDirection(RIGHT_DOWN);
				break;
			default:
				SetVelX(GetVelX());
				SetVelY(GetVelY());
			}
			SetAnimation(0,0);
		}
	}
	else if( collidingObject->GetType( ) == OBJ_ENERGY )
	{
		switch(((CPickup*)collidingObject)->GetEleType())
		{
		case 0:
			if(GetPlayerID() == 1)
			{
				collidingObject->SetActive(false);
				m_nFireEnergy += 10;
			}
			break;
		case 1:
			if(GetPlayerID() == 1)
			{
				collidingObject->SetActive(false);
				m_nWaterEnergy += 10;
			}
			break;
		case 2:
			if(GetPlayerID() == 2 || GetPlayerCount() < 2)
			{
				collidingObject->SetActive(false);
				m_nWindEnergy += 10;
			}
			break;
		case 3:
			if(GetPlayerID() == 2 || GetPlayerCount() < 2)
			{
				collidingObject->SetActive(false);
				m_nEarthEnergy += 10;
			}
		}
	}
	else if(collidingObject->GetType() == OBJ_T3SPELL)
	{
		collidingObject->SetActive(false);
		m_nTierThree += 1;
	}
}
void CPlayer::ToggleReticle()
{
	if(!m_pReticle)
	{
		m_pReticle = new CBase();
		m_pReticle->SetPosX(GetPosX());
		m_pReticle->SetPosY(GetPosY() - 32);
		if(GetPlayerID() == 1)
			m_pReticle->SetImage( CSGD_TextureManager::GetInstance()->
			LoadTexture("resource/graphics/Reticle.png", D3DXCOLOR(1,0,1,1)));
		else
			m_pReticle->SetImage( CSGD_TextureManager::GetInstance()->
			LoadTexture("resource/graphics/Reticle2.png", D3DXCOLOR(1,0,1,1)));
		m_pReticle->SetType(OBJ_RETICLE);
		m_pReticle->SetWidth(32);
		m_pReticle->SetHeight(32);
		Corona_ObjectManager::GetInstance()->AddObject(m_pReticle);
	}
	else
	{
		Corona_ObjectManager::GetInstance()->RemoveObject(m_pReticle);
		m_pReticle->Release();
		m_pReticle = NULL;
	}
}
void CPlayer::HandleEvent( CEvent * pEvent )
{
	if( pEvent->GetEventID( ) == "TileDestroyed" && ( CPlayer* )( pEvent->GetData1( ) ) == this )
	{
		if( !CSGD_WaveManager::GetInstance( )->IsWavePlaying
			( CGame::GetInstance( )->GetTileDestroyedSound( ) ) )
			CSGD_WaveManager::GetInstance( )->Play
			( CGame::GetInstance( )->GetTileDestroyedSound( ) );

		m_nScore = m_nScore + 1;
	}
	else if( pEvent->GetEventID( ) =="EnemyDied" )
		m_nScore = m_nScore + 5;
}
