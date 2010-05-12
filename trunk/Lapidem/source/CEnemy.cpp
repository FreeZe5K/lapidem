#include "CEnemy.h"
#include "CSpell.h"
#include "CFire.h"
#include "IAIState.h"
#include "AIStateEarth.h"
#include "AIStateFire.h"
#include "CGameplayState.h"

#include "CPickups.h"
#include "CAnimation.h"
#include "CTerrainBase.h"
#include "StickyNumbers.h"

#include "Corona_EventHandler.h"
#include "Corona_ObjectManager.h"
#include "Wrappers/CSGD_TextureManager.h"

CEnemy::CEnemy( EleType ElementToBe, float initx, float inity )
{
	m_nType         = OBJ_ENEMY;
	m_bIsFrozen     = false;
	m_fFrozenSpeed  = 0.5f;
	m_fFreezeTimer  = 0.0f;

	switch( ElementToBe )
	{
	case OBJ_EARTH:
		{
			currState = new AIStateEarth( );
			SetPosX(initx);
			SetPosY(inity);
			SetVelX(25.0f);
			SetVelY(0.0f);
			( ( AIStateEarth* )currState )->SetInitPos( int( GetPosX( ) ), int( GetPosY( ) ) );
			SetImage( CSGD_TextureManager::GetInstance( )->LoadTexture( "resource/graphics/lapidem_lulzenemy.png" ) );
			SetHeight( 64 );
			SetWidth( 16 );

			m_nHealth      = 80 + (CSpellFactory::GetInstance()->GetEarthLevel() * 3);
			m_SpellType    = OBJ_EARTH;
			currDirec      = RIGHT;
			currAnimation  = NULL;
		} break;
	case OBJ_FIRE:
		{
			currState = new AIStateFire( );
			SetPosX(initx);
			SetPosY(inity);
			SetVelX(75.0f);
			SetVelY(0.0f);
			SetImage( CSGD_TextureManager::GetInstance( )->LoadTexture( "resource/graphics/lapid_lulzfireenemy.png" ) );
			SetHeight( 64 );
			SetWidth ( 32 );

			m_nHealth      = 50 + (CSpellFactory::GetInstance()->GetFireLevel() * 2);
			m_SpellType    = OBJ_FIRE;
			currDirec      = RIGHT;
			currAnimation  = NULL;

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
	m_fKnockBack   = 0.0f;
	m_bKnockBack   = false;
	m_bBurning     = false;
	m_nBurnDamage  = 0;
	m_fBurnTimer   = 0.0f;
}

CEnemy::~CEnemy( )
{
	if( currState ) 
		delete currState;

	CPickup * newpickup = new CPickup();
	newpickup->SetPosX(GetPosX());
	newpickup->SetPosY(GetPosY());
	newpickup->SetActive(true);

	//if(!(rand() % 1023))
	//{
	//	newpickup->SetActive(false);
	//}
	//else
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
	if(!m_bKnockBack)
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
	}
	else
	{
		if(m_fKnockBack < 0)
		{
			m_bKnockBack = false;
		}
		else
		{
			CCharacter::Update(fElapsedTime);
			m_fKnockBack-=fElapsedTime * 100;
		}
	}

	if(m_bBurning)
	{
		m_fBurnTimer -= fElapsedTime;

		if(m_fBurnTimer <= 0)
		{
			m_bBurning = false;
			m_nBurnDamage = 0;
		}

		if(!((int)m_fBurnTimer %  3))
		{
			m_fBurnTimer -= 1.0f;
			StickyNumbers* SN = new StickyNumbers();
			SN->SetTimer(5.0f);
			SN->SetPosX( GetPosX());
			SN->SetPosY( GetPosY() - 24);
			char buffer[4];
			sprintf_s(buffer, 4, "%i", TakeDamage(m_nBurnDamage));
			SN->SetText(buffer);
			SN->SetVelY(-30);

			Corona_ObjectManager::GetInstance()->AddObject(SN);
			SN->Release();
		}
	}

	if( m_bIsFrozen )
	{
		m_fFreezeTimer = m_fFreezeTimer - fElapsedTime;

		if( m_fFreezeTimer <= 0 )
			m_bIsFrozen = false;

		SetVelX( m_fFrozenSpeed );
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
		int DamageToTake = ((CSpell*)collidingObject)->GetDamage();
		int EleType	= GetEleType();

		if( ( spelltype == OBJ_FIRE && EleType == OBJ_ICE ) || 
			( spelltype == OBJ_ICE && EleType == OBJ_WIND ) ||
			( spelltype == OBJ_EARTH && EleType == OBJ_FIRE ) || 
			( spelltype == OBJ_WIND && EleType == OBJ_EARTH ) )
		{
			StickyNumbers* SN = new StickyNumbers();
			SN->SetTimer(20.0f);
			SN->SetPosX( GetPosX());
			SN->SetPosY( GetPosY() - 24);

			//char * buffer = new char[4];//NULL;
			char buffer[4];

			sprintf_s(buffer, 4, "%i", TakeDamage(DamageToTake<<1));
			SN->SetText(buffer);
			SN->SetVelY(-30);

			Corona_ObjectManager::GetInstance()->AddObject(SN);
			SN->Release();
		}
		else if( (spelltype == OBJ_FIRE && EleType == OBJ_EARTH) || 
			(spelltype == OBJ_ICE && EleType == OBJ_FIRE) ||
			(spelltype == OBJ_EARTH && EleType == OBJ_WIND ) || 
			(spelltype == OBJ_WIND && EleType == OBJ_ICE) )
		{
			StickyNumbers* SN = new StickyNumbers();
			SN->SetTimer(5.0f);
			SN->SetPosX( GetPosX());
			SN->SetPosY( GetPosY() - 24);

			//char * buffer = new char[4];//NULL;
			char buffer[4];

			sprintf_s(buffer, 4, "%i", TakeDamage(DamageToTake>>1));
			SN->SetText(buffer);
			SN->SetVelY(-20);

			Corona_ObjectManager::GetInstance()->AddObject(SN);
			SN->Release();
		}
		else if(spelltype != EleType)
		{
			StickyNumbers* SN = new StickyNumbers();
			SN->SetTimer(10.0f);
			SN->SetPosX( GetPosX());
			SN->SetPosY( GetPosY() - 24);

			//char * buffer = new char[4];//NULL;
			char buffer[4];

			sprintf_s(buffer, 4, "%i", TakeDamage(DamageToTake));
			SN->SetText(buffer);
			SN->SetVelY(-25);

			Corona_ObjectManager::GetInstance()->AddObject(SN);
			SN->Release();
		}

		if( OBJ_FIRE == spelltype )
		{
			if(m_bBurning)
				m_nBurnDamage += ((CFire*)collidingObject)->GetDOT();
			else
			{
				m_bBurning = true;
				m_nBurnDamage += ((CFire*)collidingObject)->GetDOT();
				m_fBurnTimer = 15.0f;
			}
		}

		if( OBJ_ICE == spelltype )
		{
			m_bIsFrozen     = true;
			m_fFrozenSpeed  = GetVelX( ) * 0.6f;
			m_fFreezeTimer  = 15.0f;
		}
	}
}
