#include "stdheaders.h"
#include "CEnemy.h"
#include "CSpell.h"
#include "CFire.h"
#include "IAIState.h"
#include "AIStateEarth.h"
#include "AIStateFire.h"
#include "AIStateWind.h"
#include "AIStateIce.h"
#include "AIDocBoss.h"
#include "AISisBoss.h"

#include "CPickups.h"
#include "CAnimation.h"
#include "CTerrainBase.h"
#include "StickyNumbers.h"


#define CHANGE_ANIM if(m_fChangeAnimationTimer >= 1.0f)

CEnemy::CEnemy( EleType ElementToBe, float initx, float inity, int boss, CFlock* Flock )
{
	m_nType         = OBJ_ENEMY;
	m_bIsFrozen     = false;
	m_fFrozenSpeed  = 0.5f;
	m_fFreezeTimer  = 0.0f;
	m_fChangeAnimationTimer = 0.76f;


	if(!boss)
	{
	switch( ElementToBe )
	{
	case OBJ_EARTH:
		{
			currState = new AIStateEarth( );
			SetHeight( 100 );
			SetWidth( 135 );
			SetPosX(initx);
			SetPosY(inity - GetHeight());
			SetVelX(25.0f);
			SetVelY(0.0f);
			( ( AIStateEarth* )currState )->SetInitPos( int( GetPosX( ) ), int( GetPosY( ) ) );
			SetHeight( 100 );
			SetWidth( 135 );
			m_nHealth      = 80 * CGameplayState::GetInstance()->GetDifficulty() + (CSpellFactory::GetInstance()->GetWindLevel() * 10);
			m_SpellType    = OBJ_EARTH;
			currDirec      = RIGHT;
			m_nAnimation = m_SpellType +1;
			m_fScale = .8f;
			SetAnimation(OBJ_EARTH +1,0);
		} break;
	case OBJ_FIRE:
		{
			currState = new AIStateFire( );
			SetPosX(initx);
			SetPosY(inity - 55);
			SetVelX(75.0f);
			SetVelY(0.0f);
			SetHeight( 54 );
			SetWidth ( 16 );
			m_nHealth      = 50 * CGameplayState::GetInstance()->GetDifficulty() + (CSpellFactory::GetInstance()->GetEarthLevel() * 7);
			m_SpellType    = OBJ_FIRE;
			currDirec      = RIGHT;
			m_fScale = 0.5f;
			m_nAnimation = m_SpellType +1;
			SetAnimation(OBJ_FIRE +1,0);

		} break;
	case OBJ_ICE:
		{
			currState = new AIStateIce( );
			SetPosX(initx);
			SetPosY(inity - 55);
			SetVelX(50.0f);
			SetVelY(0.0f);
			SetHeight( 64 );
			SetWidth ( 64 );
			SetAnimation(1,0);
			m_nHealth      = 50 * CGameplayState::GetInstance()->GetDifficulty() + (CSpellFactory::GetInstance()->GetFireLevel() * 7);
			m_SpellType    = OBJ_ICE;
			m_fScale = 0.5f;
			currDirec      = RIGHT;
			m_nAnimation = m_SpellType +1;
			SetAnimation(OBJ_ICE +1,0);
		} break;
	case OBJ_WIND:
		{
			currState = new AIStateWind();
			SetPosX(initx);
			SetPosY(inity);
			m_fScale = 0.4f;
			
			((AIStateWind*)currState)->SetFlock((CFlock*)Flock);
			SetVelX((float)(rand()%150));
			SetVelY((float)(rand()%150));
			if(rand()%2)
			{
				SetVelX((float)(rand()%150));
				SetVelY((float)(rand()%150));
			}
			else
			{
				SetVelX(rand()%150 * -1.0f);
				SetVelY(rand()%150 * -1.0f);
			}
			
			SetHeight(16);
			SetWidth(16);
			m_nHealth = 25 * CGameplayState::GetInstance()->GetDifficulty() + CSpellFactory::GetInstance()->GetIceLevel() * 5;
			m_SpellType = OBJ_WIND;
			currDirec = RIGHT;
			m_nAnimation = m_SpellType +1;
			SetAnimation(OBJ_WIND +1,0);
			currAnimation = NULL;
			break;
		} 
	}
	}


	else if(boss  == 1)
	{
		currState = new AIDocBoss(1500 + (CSpellFactory::GetInstance()->GetIceLevel() + CSpellFactory::GetInstance()->GetWindLevel() + CSpellFactory::GetInstance()->GetEarthLevel() + CSpellFactory::GetInstance()->GetFireLevel()) * 15);

		SetPosX(initx);
		SetPosY(inity);
		SetVelX(0.0f);
		SetVelY(0.0f);
		SetHeight(64);
		SetWidth(40);
		m_nAnimation = 5;
		SetImage(CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/Doctorboss.png"));
		m_nHealth = 1500 + (CSpellFactory::GetInstance()->GetIceLevel() + CSpellFactory::GetInstance()->GetWindLevel() + CSpellFactory::GetInstance()->GetEarthLevel() + CSpellFactory::GetInstance()->GetFireLevel()) * 15;
		m_SpellType = OBJ_SHIELD;
		m_fScale = 1.0f;
		currDirec = RIGHT;
		SetAnimation(5,0);
	}
	else if(boss >= 2)
	{
		currState = new AISisBoss;

		SetPosX(initx);
		SetPosY(inity);
		SetVelX(0.0f);
		SetVelY(0.0f);
		SetHeight(60);
		m_fScale = 1.0f;
		SetWidth(50);
		SetImage(CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/Lapidem_WomanBoss.png", D3DCOLOR_XRGB(0, 0, 0)));
		m_nHealth = 1500;
		m_SpellType = OBJ_EARTH;
		currDirec = RIGHT;
		m_nAnimation = 6;
		SetAnimation(6,0);
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
	if(m_SpellType == OBJ_WIND)
	{
		if(dynamic_cast<AIStateWind*>(currState))
		{
			if(((AIStateWind*)currState)->GetFlock())
			{
				((AIStateWind*)currState)->GetFlock()->RemoveMember(this);
			}
		}
	}
	if( currState ) 
		delete currState;

	CPickup * newpickup = new CPickup();
	newpickup->SetPosX(GetPosX());
	newpickup->SetPosY(GetPosY());
	newpickup->SetActive(true);

	if(!(rand() % 20))
	{
		newpickup->SetType(OBJ_T3SPELL);
		newpickup->SetImage( CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/Lapid_SuperEnergy.png"));
		newpickup->SetWidth(32);
		newpickup->SetHeight(32);

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
	if( ! ( CGameplayState::GetInstance()->GetLevel()->GetTile((int)GetPosX(), (int)GetPosY() ) ) )
	{
		SetHealth(0);
	}
	if(m_nHealth >0)
	{
		if(!m_bKnockBack)
		{
			m_fShotTimer = m_fShotTimer - fElapsedTime;

			if( 0.0f == m_fWaitTimer )
			{	
				if(m_SpellType == OBJ_WIND)
				{
					CBase::Update( fElapsedTime );
				}
				else
				{
					CCharacter::Update( fElapsedTime );
				}

				m_nAttackWho = currState->Update( fElapsedTime, this );

				if( m_nAttackWho && m_fShotTimer < 0 )
				{
					m_fWaitTimer += fElapsedTime;
					m_fShotTimer = 2.0f;
					animation->Reset();
					SetAnimation(m_nAnimation,2);
					
				}
			}
			else
			{
				m_fWaitTimer = m_fWaitTimer + fElapsedTime;

				/*if(m_SpellType !=OBJ_WIND)
					SetPosY( GetPosY( ) + 150.0f * fElapsedTime );*/

				
				char* pleasework = animation->GetTrigger();
				if(strcmp(pleasework, "Done") == 0)
				{	
					if( 1 == m_nAttackWho )
					{
						currState->Attack( CGameplayState::GetInstance( )->GetPlayerOne( ), this );
					}
					else if( 2 == m_nAttackWho )
					{
						currState->Attack( CGameplayState::GetInstance( )->GetPlayerTwo( ), this );
					}
					animation->Reset();
					SetAnimation(m_nAnimation,0);
					
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
				char buffer[16];
				sprintf_s(buffer, 16, "%i", TakeDamage(m_nBurnDamage));
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
	}
	else
	{
		if(currAnimation != 1)
		{
			animation->Reset();
			SetAnimation(m_nAnimation,1);
		}
		char* pleasework = animation->GetTrigger();
		if(strcmp(pleasework,"Dead") ==0)
		{
			Corona_EventHandler::GetInstance( )->SendEvent( "EnemyDied", ( void* )this );
			SetActive( false );
		}
	}
	animation->Update(fElapsedTime);
	CHANGE_ANIM
	{
		if(GetVelX() <30)
		{
			IsRotated = true;
		}
		else if(GetVelX() >-30)
		{
			IsRotated = false;
		}
		m_fChangeAnimationTimer =0.0f;
	}
	else
	{
		m_fChangeAnimationTimer += fElapsedTime;
	}
}

void CEnemy::Render()
{
	if(dynamic_cast<AIStateWind*>(currState))
	{
		if( animation )
		{
			if( animation->GetImageID( ) != -1 && IsRotated )
				CSGD_TextureManager::GetInstance( )->Draw( animation->GetImageID( ), 
				int( GetPosX( ) - CCamera::GetCamera( )->GetXOffset( ) ),
				int( GetPosY( ) - CCamera::GetCamera( )->GetYOffset( ) ),
				m_fScale, m_fScale, &animation->GetFrames( )->DrawRect );
			else if( animation->GetImageID( ) != -1 && !IsRotated )
			{
				CSGD_TextureManager::GetInstance( )->Draw( animation->GetImageID( ), 
					int( GetPosX( ) - CCamera::GetCamera( )->GetXOffset( ) + GetWidth( ) ), 
					int( GetPosY( ) - CCamera::GetCamera( )->GetYOffset( ) ),
					-m_fScale, m_fScale, &animation->GetFrames( )->DrawRect );
			} else CBase::Render( );
		} 
	}
	else
	{
		if( animation )
		{
			if( animation->GetImageID( ) != -1 && IsRotated )
				CSGD_TextureManager::GetInstance( )->Draw( animation->GetImageID( ), 
				int( GetPosX( ) - CCamera::GetCamera( )->GetXOffset( ) ),
				int( GetPosY( ) - CCamera::GetCamera( )->GetYOffset( ) ),
				m_fScale, m_fScale, &animation->GetFrames( )->DrawRect );
			else if( animation->GetImageID( ) != -1 && !IsRotated )
			{
				CSGD_TextureManager::GetInstance( )->Draw( animation->GetImageID( ), 
					int( GetPosX( ) - CCamera::GetCamera( )->GetXOffset( ) + GetWidth( ) ), 
					int( GetPosY( ) - CCamera::GetCamera( )->GetYOffset( ) ),
					-m_fScale, m_fScale, &animation->GetFrames( )->DrawRect );
			} else CBase::Render( );
		} 
	}
}

RECT CEnemy::GetCollisionRect(float fElapsedTime)
{
	RECT temp = animation->GetFrames()->CollisionRect;
	RECT draw = animation->GetFrames()->DrawRect;
	RECT pleasework;

	if(IsRotated)
	{
		pleasework.left    = LONG( ( temp.left - draw.left )* m_fScale );
		pleasework.right   = LONG( pleasework.left + ( ( temp.right - temp.left) * m_fScale) );
		pleasework.top     = LONG( ( temp.top - draw.top ) * m_fScale );
		pleasework.bottom  = LONG( pleasework.top + ( ( temp.bottom - temp.top ) * m_fScale ) );
	}
	else
	{
		POINT anchor = animation->GetFrames()->AnchorPoint;

		pleasework.left    = LONG( ( ( temp.left - draw.left ) * m_fScale )  - ( ( temp.left - anchor.x ) * m_fScale ) );
		pleasework.right   = LONG( pleasework.left + ( ( temp.right - temp.left ) * m_fScale ) );
		pleasework.top     = LONG( ( temp.top - draw.top ) * m_fScale );
		pleasework.bottom  = LONG( pleasework.top + ( ( temp.bottom - temp.top ) *m_fScale ) );
	}

	pleasework.left        += LONG( GetPosX( ) + GetVelX( ) * fElapsedTime );
	pleasework.right       += LONG( GetPosX( ) + GetVelX( ) * fElapsedTime );
	pleasework.top         += LONG( GetPosY( ) + GetVelY( ) * fElapsedTime );
	pleasework.bottom      += LONG( GetPosY( ) + GetVelY( ) * fElapsedTime );

	SetWidth(pleasework.right - pleasework.left + temp.left- draw.left);
	SetHeight(draw.bottom- draw.top+ temp.top - draw.top);

	return pleasework;
}

void CEnemy::HandleCollision(float fElapsedTime, CBase* collidingObject )
{

	if(collidingObject->GetType() < 0 || GetHealth() <= 0)
		return;

	if( collidingObject->GetType() == OBJ_TERRA )
	{
		if(m_SpellType == OBJ_WIND)
		{
			RECT r;
			IntersectRect( &r, & this->GetCollisionRect( fElapsedTime ), &collidingObject->GetCollisionRect( fElapsedTime ) );

			int nRectWidth    = r.right - r.left;
			int nRectHeight   = r.bottom - r.top;

			if( nRectHeight > nRectWidth )
			{
				if( this->GetPosX( ) > collidingObject->GetPosX( ) )
					SetPosX( GetPosX( ) + nRectWidth );
				else if ( this->GetPosX() < collidingObject->GetPosX( ) )
					SetPosX( GetPosX( ) - nRectWidth );
				SetVelY(-GetVelY());
			}
			else
			{
				if( this->GetPosY( ) > collidingObject->GetPosY( ) )
					SetPosY( GetPosY( ) + nRectHeight );
				else if(this->GetPosY( ) < collidingObject->GetPosY( ) )
				{
					SetPosY( GetPosY( ) - nRectHeight );
				}
				SetVelX(-GetVelX());
			}
		}
		else
		{

			int TerraType = ( ( CTerrainBase* )collidingObject )->GetTypeTerrain( );

			RECT r;
			IntersectRect( &r, & this->GetCollisionRect( fElapsedTime ), &collidingObject->GetCollisionRect( fElapsedTime ) );

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
				{
					SetPosY( GetPosY( ) - nRectHeight );
					SetVelY(0.0);
				}
			}
		}
	}
	else if( collidingObject->GetType( ) == OBJ_SPELL && ( ( CSpell* )collidingObject )->PlayerShot( ) )
	{
		int spelltype = ((CSpell*)collidingObject)->GetElement();
		int DamageToTake = ((CSpell*)collidingObject)->GetDamage();
		int EleType	= GetEleType();
		
		if( EleType == OBJ_SHIELD)
			return;
		else if(EleType == OBJ_NONE)
			if(spelltype != OBJ_EARTH)
			{
				StickyNumbers* SN = new StickyNumbers();
				SN->SetTimer(20.0f);
				SN->SetPosX( GetPosX());
				SN->SetPosY( GetPosY() - 24);

				char buffer[4];

				sprintf_s(buffer, 4, "%i", TakeDamage(DamageToTake));
				SN->SetText(buffer);
				SN->SetVelY(-30);

				Corona_ObjectManager::GetInstance()->AddObject(SN);
				SN->Release();
			}
			else
				return;

		if( ( spelltype == OBJ_FIRE && EleType == OBJ_ICE ) || 
			( spelltype == OBJ_ICE && EleType == OBJ_WIND ) ||
			( spelltype == OBJ_EARTH && EleType == OBJ_FIRE ) || 
			( spelltype == OBJ_WIND && EleType == OBJ_EARTH ) )
		{
			StickyNumbers* SN = new StickyNumbers();
			SN->SetTimer(20.0f);
			SN->SetPosX( GetPosX());
			SN->SetPosY( GetPosY() - 24);

			char buffer[16];

			sprintf_s(buffer, 16, "%i", TakeDamage(DamageToTake<<1));
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
			char buffer[16];

			sprintf_s(buffer, 16, "%i", TakeDamage(DamageToTake>>1));
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

			char buffer[16];

			sprintf_s(buffer, 16, "%i", TakeDamage(DamageToTake));
			SN->SetText(buffer);
			SN->SetVelY(-25);

			Corona_ObjectManager::GetInstance()->AddObject(SN);
			SN->Release();
		}

		if( OBJ_FIRE == spelltype && OBJ_FIRE != EleType )
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
