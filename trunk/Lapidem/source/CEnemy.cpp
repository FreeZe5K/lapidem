#include "CEnemy.h"
#include "CPickups.h"
#include "CAnimation.h"
#include "IAIState.h"
#include "AIStateEarth.h"
#include "CTerrainBase.h"
#include "Corona_EventHandler.h"
#include "Corona_ObjectManager.h"
#include "CSpell.h"
#include "Wrappers/CSGD_TextureManager.h"
#include "CGameplayState.h"

CEnemy::CEnemy( EleType ElementToBe, float initx, float inity )
{
	m_nType = OBJ_ENEMY;

	switch( ElementToBe )
	{
	case OBJ_EARTH:
		{
			currState = new AIStateEarth( );
			SetPosX(initx);
			SetPosY(inity);
			SetVelX(100.0f);
			SetVelY(0.0f);
			( ( AIStateEarth* )currState )->SetInitPos( int( GetPosX( ) ), int( GetPosY( ) ) );
			SetImage( CSGD_TextureManager::GetInstance( )->LoadTexture( "resource/graphics/lapidem_lulzenemy.png" ) );
			SetHeight( 64 );
			SetWidth( 16 );

			m_nHealth      = 80;
			m_SpellType    = OBJ_EARTH;
			currDirec      = RIGHT;
			currAnimation  = NULL;
		} break;
	case OBJ_FIRE:
		{
		} break;
	case OBJ_ICE:
		{
		} break;
	case OBJ_WIND:
		{
		} break;
	}

	m_fShotTimer   = 3.0f;
	m_fWaitTimer   = 0.0f;
	m_nAttackWho   = 0;
}

CEnemy::~CEnemy( )
{
	if( currState ) delete currState;

	CPickup * newpickup = new CPickup();
	newpickup->SetPosX(GetPosX());
	newpickup->SetPosY(GetPosY());
	newpickup->SetActive(true);

	if(!(rand() % 1023))
	{
		newpickup->SetActive(false);
	}
	else
	{
		newpickup->SetType(OBJ_ENERGY);
		if(GetEleType() == OBJ_EARTH)
		{
			newpickup->SetEleType(OBJ_EARTH);
			newpickup->SetImage( CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/Lapid_EarthEnergy.png"));
			newpickup->SetWidth(64);
			newpickup->SetHeight(48);

		}
		else if(GetEleType() == OBJ_FIRE)
		{
			newpickup->SetEleType(OBJ_FIRE);
			newpickup->SetImage( CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/Lapid_FireEnergy.png"));
			newpickup->SetWidth(64);
			newpickup->SetHeight(48);

		}
		else if(GetEleType() == OBJ_ICE)
		{
			newpickup->SetEleType(OBJ_ICE);
			newpickup->SetImage( CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/Lapid_IceEnergy.png"));
			newpickup->SetWidth(64);
			newpickup->SetHeight(48);

		}
		else
		{
			newpickup->SetEleType(OBJ_WIND);
			newpickup->SetImage( CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/Lapid_WindEnergy.png"));
			newpickup->SetWidth(64);
			newpickup->SetHeight(58);

		}
	}
		Corona_ObjectManager::GetInstance()->AddObject(newpickup);
		newpickup->Release();

}

void CEnemy::Update( float fElapsedTime )
{
	m_fShotTimer = m_fShotTimer - fElapsedTime;

	if( 0.0f == m_fWaitTimer )
	{	
		m_nAttackWho = currState->Update( fElapsedTime, this );

		if( m_nAttackWho && m_fShotTimer < 0 )
		{
			m_fWaitTimer += fElapsedTime;
			m_fShotTimer = 2.0f;
		}

		CCharacter::Update( fElapsedTime );
	}
	else
	{
		m_fWaitTimer = m_fWaitTimer + fElapsedTime;
		SetPosY( GetPosY( ) + 150.0f * fElapsedTime );

		if( m_fWaitTimer > 0.5f )
		{
			if( 1 == m_nAttackWho )
				currState->Attack( CGameplayState::GetInstance( )->GetPlayerOne( ), this );
			else if( 2 == m_nAttackWho )
				currState->Attack( CGameplayState::GetInstance( )->GetPlayerTwo( ), this );
			m_fWaitTimer = 0.0f;
		}
	}

	if( m_nHealth <= 0 )
	{
		Corona_EventHandler::GetInstance( )->SendEvent( "EnemyDied", ( void* )this );
		SetActive( false );
	}
}

void CEnemy::HandleCollision( CBase* collidingObject )
{
	if( collidingObject->GetType() == OBJ_TERRA )
	{
		//if(((CTerrainBase*)collidingObject)->GetTypeTerrain() == T_EMPTY)
		//	SetVelX(-GetVelX());

		int TerraType = ( ( CTerrainBase* )collidingObject )->GetTypeTerrain( );

		RECT r;
		IntersectRect( &r, & this->GetCollisionRect( 0 ), &collidingObject->GetCollisionRect( 0 ) );

		int nRectWidth    = r.right - r.left;
		int nRectHeight   = r.bottom - r.top;

		if( nRectHeight > nRectWidth )
		{
			if( this->GetPosX( ) > collidingObject->GetPosX( ) )
				SetPosX( GetPosX( ) + nRectWidth );
			else if ( this->GetPosX() < collidingObject->GetPosX( ) )
				SetPosX( GetPosX( ) - nRectWidth );
		}
		else if( nRectHeight < nRectWidth ) 
		{
			if( this->GetPosY( ) > collidingObject->GetPosY( ) )
				SetPosY( GetPosY( ) + nRectHeight );
			else if(this->GetPosY( ) < collidingObject->GetPosY( ) )
				SetPosY( GetPosY( ) - nRectHeight );
		}
	}
	else if( collidingObject->GetType( ) == OBJ_SPELL && ( ( CSpell* )collidingObject )->PlayerShot( ) )
	{
		int spelltype = ((CSpell*)collidingObject)->GetElement();
		if(spelltype == OBJ_FIRE)
		{
			if(GetEleType() == OBJ_ICE)
			{
				m_nHealth = m_nHealth - (((CSpell*)collidingObject)->GetDamage( )<<1);	
			}
			else if(GetEleType() == OBJ_EARTH)
			{
				m_nHealth = m_nHealth - (((CSpell*)collidingObject)->GetDamage()>>1);	
			}
			else if(GetEleType() == OBJ_WIND)
			{
				m_nHealth = m_nHealth - ((CSpell*)collidingObject)->GetDamage();
			}
		}
		else if(GetEleType() == OBJ_ICE)
		{
			if(GetEleType() == OBJ_FIRE)
			{
				m_nHealth = m_nHealth - (((CSpell*)collidingObject)->GetDamage( )>>1);	
			}
			else if(GetEleType() == OBJ_EARTH)
			{
				m_nHealth = m_nHealth - ((CSpell*)collidingObject)->GetDamage();	
			}
			else if(GetEleType() == OBJ_WIND)
			{
				m_nHealth = m_nHealth - (((CSpell*)collidingObject)->GetDamage() <<1);
			}
		}
		else if(GetEleType() == OBJ_EARTH)
		{
			if(GetEleType() == OBJ_FIRE)
			{
				m_nHealth = m_nHealth - (((CSpell*)collidingObject)->GetDamage( )<<1);	
			}
			else if(GetEleType() == OBJ_ICE)
			{
				m_nHealth = m_nHealth - ((CSpell*)collidingObject)->GetDamage();	
			}
			else if(GetEleType() == OBJ_WIND)
			{
				m_nHealth = m_nHealth - (((CSpell*)collidingObject)->GetDamage() >>1);
			}
		}
		else if(GetEleType() == OBJ_WIND)
		{
			if(GetEleType() == OBJ_FIRE)
			{
				m_nHealth = m_nHealth - (((CSpell*)collidingObject)->GetDamage( ));	
			}
			else if(GetEleType() == OBJ_EARTH)
			{
				m_nHealth = m_nHealth - (((CSpell*)collidingObject)->GetDamage()<<1);	
			}
			else if(GetEleType() == OBJ_ICE)
			{
				m_nHealth = m_nHealth - (((CSpell*)collidingObject)->GetDamage() >>1);
			}
		}

	}
	//else if( collidingObject->GetType( ) == OBJ_PLAYER )
	//	SetVelX( -GetVelX( ) );
}