//////////////////////////////////////////////////////////////////////////
//  File Name   :   "CSpellFactory.cpp"
//
//  Author Name :   Jonathan Caro
//
//  Purpose     :   Used to create all spells. Also handles the experience
//					gains and levels of each element. After Creation, the
//					spell is put into the ObjectFactory as well as 
//					returned.
//////////////////////////////////////////////////////////////////////////
#include "CSpellFactory.h"

#include "Corona_ObjectManager.h"
#include "CCharacter.h"
#include "CFire.h"
#include "CIce.h"
#include "CEarth.h"
#include "CWind.h"
#include "CCamera.h"
#include "CParticleManager.h"
#include "CGameplayState.h"

CSpellFactory* CSpellFactory::m_pSF = NULL;

CSpellFactory::CSpellFactory()
{
	m_nEarthLVL = 0;
	m_nWindLVL = 0;
	m_nFireLVL = 0;
	m_nIceLVL = 0;

	m_nEarthXP = 0;
	m_nWindXP = 0;
	m_nFireXP = 0;
	m_nIceXP = 0;

	m_pEF = CEmitterFactory::GetInstance();
}

CSpellFactory::~CSpellFactory()
{
	if(m_pEF)
	{
		m_pEF = NULL;
	}
	if( m_pEF )
	{
		m_pEF->UnloadAll( );
		m_pEF = NULL;
	}
}

void CSpellFactory::AddWindXP(int nXP)
{
	m_nWindXP += nXP;
	if(m_nWindXP> (35*(1.5*m_nWindLVL)))
	{
		m_nWindXP -=(int)(35*(1.5f*m_nWindLVL));
		m_nWindLVL++;
	}
}
void CSpellFactory::AddEarthXP(int nXP)
{
	m_nEarthXP += nXP;
	if(m_nEarthXP > (25+(40* m_nEarthLVL * 0.5)))
	{
		m_nEarthXP -= (int)(25+(40* m_nEarthLVL * 0.5f));
		m_nEarthLVL++;
	}
}
void CSpellFactory::AddFireXP(int nXP)
{
	m_nFireXP+= nXP;

	if(m_nFireXP > 50 * (m_nFireLVL +2))
	{
		m_nFireXP -= (50* (m_nFireLVL +2));
		m_nFireLVL++;
	}

}
void CSpellFactory::AddIceXP(int nXP)
{
	m_nIceXP+= nXP;

	if(m_nIceXP > 50 * (m_nIceLVL +2))
	{
		m_nIceXP -= (50* (m_nIceLVL +2));
		m_nIceLVL++;
	}
}


void CSpellFactory::CreateEarth(CCharacter* pShooter, int nTier)
{
	if(pShooter->GetType() == OBJ_PLAYER)
	{
		switch(nTier)
		{
		case 1: // First Tier... Basic Boulder
			{
				CEarth* newearth = new CEarth();
				newearth->SetPosX(pShooter->GetPosX() + pShooter->GetWidth() + 2);
				newearth->SetPosY(pShooter->GetPosY());

				if(pShooter->GetType() == OBJ_PLAYER && ((CPlayer*)pShooter)->GetReticle())
				{
					CBase* tempRet = ((CPlayer*)pShooter)->GetReticle();

					float speedx = (tempRet->GetPosX() - pShooter->GetPosX()) / 50;
					float speedy = (tempRet->GetPosY() - pShooter->GetPosY()) / 50;

					newearth->SetVelX(100 * speedx);
					newearth->SetVelY(100 * speedy);

					if(newearth->GetVelX() < 0)
						newearth->SetPosX(pShooter->GetPosX() - 34);
				}
				else
				{
					DIRECTION wheretoshoot = pShooter->GetDirection();
					switch(wheretoshoot)
					{
					case 0:
						{
							newearth->SetVelX(-150);
							newearth->SetPosX(pShooter->GetPosX() - 34);
							newearth->SetVelY(0);
							newearth->SetDirection(LEFT);
							break;
						}
					case 1:
						{
							newearth->SetVelX(150);
							newearth->SetVelY(0);
							break;
						}
					case 2:
						{
							newearth->SetVelX(0);
							newearth->SetVelY(-150);
							break;
						}
					case 3:
						{
							newearth->SetVelX(0);
							newearth->SetVelY(150);
							break;
						}
					case 4:
						{
							newearth->SetVelX(-75);
							newearth->SetPosX(pShooter->GetPosX() - 34);
							newearth->SetVelY(-75);
							newearth->SetDirection(LEFT);
							break;
						}
					case 5:
						{
							newearth->SetVelX(-75);
							newearth->SetPosX(pShooter->GetPosX() - 34);
							newearth->SetDirection(LEFT);
							newearth->SetVelY(75);
							break;
						}
					case 6:
						{
							newearth->SetVelX(75);
							newearth->SetVelY(-75);
							break;
						}
					case 7:
						{
							newearth->SetVelX(75);
							newearth->SetVelY(75);
							break;
						}
					}
				}
				newearth->SetDamage(20 + 4* m_nEarthLVL);
				newearth->SetLifespan(10.0f + 1.5f* m_nEarthLVL);
				newearth->SetActive(true);
				newearth->SetTier(nTier);
				newearth->ShotBy(true);	
				newearth->SetElement(OBJ_EARTH);
				newearth->SetHeight(32);
				newearth->SetWidth(32);	
				Corona_ObjectManager::GetInstance()->AddObject(newearth);
				newearth->Release();
				break;
			}
		case 2:
			{
				CEarth* newearth = new CEarth();
				newearth->SetPosX(pShooter->GetPosX());
				newearth->SetPosY(pShooter->GetPosY() + pShooter->GetHeight());
				newearth->SetVelX(0.0f);
				newearth->SetVelY(-75.0f);
				newearth->SetDamage(20 + 4* m_nEarthLVL);
				newearth->SetLifespan(15.0f + 1.5f* m_nEarthLVL);
				newearth->SetActive(true);
				newearth->SetTier(nTier);
				newearth->ShotBy(true);	
				newearth->SetElement(OBJ_EARTH);
				newearth->SetHeight(0);
				newearth->SetWidth(32);	
				newearth->SetImage(CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/Lapidem_EarthPillar.png"));
				Corona_ObjectManager::GetInstance()->AddObject(newearth);
				newearth->Release();
				break;
			}
		case 3:
			{
				CEarth* newearth = new CEarth();
				newearth->SetPosX(CCamera::GetCamera()->GetXOffset());
				newearth->SetPosY(CCamera::GetCamera()->GetHeight());
				newearth->SetVelX(0.0f);
				newearth->SetVelY(-300.0f);
				newearth->SetDamage(20 + 4* m_nEarthLVL);
				newearth->SetLifespan(300);
				newearth->SetActive(true);
				newearth->SetTier(3);
				newearth->ShotBy(true);	
				newearth->SetElement(OBJ_EARTH);
				newearth->SetHeight(CGame::GetInstance()->GetScreenHeight());
				newearth->SetWidth(CGame::GetInstance()->GetScreenWidth());	
				newearth->SetImage(CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/Lapidem_EarthHand.png", D3DCOLOR_XRGB(255,255,255)));
				Corona_ObjectManager::GetInstance()->AddObject(newearth);
				newearth->Release();
				break;
			}
		}
	}
	else
	{

		CEarth* newearth = new CEarth();
		if(nTier ==1)
		{
			newearth->SetPosX(CGameplayState::GetInstance()->GetPlayerOne()->GetPosX() + (CGameplayState::GetInstance()->GetPlayerOne()->GetWidth()>>1));
			newearth->SetPosY(CGameplayState::GetInstance()->GetPlayerOne()->GetPosY() - 100);
		}
		else if(nTier ==2)
		{
			newearth->SetPosX(CGameplayState::GetInstance()->GetPlayerTwo()->GetPosX() + (CGameplayState::GetInstance()->GetPlayerTwo()->GetWidth()>>1));
			newearth->SetPosY(CGameplayState::GetInstance()->GetPlayerTwo()->GetPosY() - 100);
		}
		newearth->SetVelX(0);
		newearth->SetVelY(100);
		newearth->SetDamage(20);
		newearth->SetLifespan(10.0);
		newearth->SetActive(true);
		newearth->SetTier(1);
		newearth->ShotBy(false);
		newearth->SetElement(OBJ_EARTH);
		newearth->SetHeight(32);
		newearth->SetWidth(32);	
		Corona_ObjectManager::GetInstance()->AddObject(newearth);
		newearth->Release();		
	}
}

void CSpellFactory::CreateEnemyFire(CCharacter * pShooter, CBase * pTarget)
{
	CFire* newfire = new CFire();

	newfire->SetElement(OBJ_FIRE);

	newfire->ShotBy(false);

	newfire->SetPosX(pShooter->GetPosX() + pShooter->GetWidth()  * .5f);
	newfire->SetPosY(pShooter->GetPosY() + pShooter->GetHeight() * .25f);
	float SpeedX =  ( (pTarget->GetPosX() + pTarget->GetWidth()  * .5f) - newfire->GetPosX() ) / 100;
	float SpeedY =  ( (pTarget->GetPosY() + pTarget->GetHeight() * .5f) - newfire->GetPosY() ) / 100;
	newfire->SetVelX( SpeedX * 250 );
	newfire->SetVelY( SpeedY * 250 );

	newfire->SetDamage(20);
	newfire->SetDOT(0);
	newfire->SetLifespan(5.0f);
	newfire->SetActive(true);
	newfire->SetTier(1);

	newfire->SetWidth(32);
	newfire->SetHeight(16);

	Corona_ObjectManager::GetInstance()->AddObject(newfire);

	CEmitter *emitter;
	emitter = m_pEF->CreateEmitter( "firespell" );
	emitter->SetPosX( newfire->GetPosX( ) - ( newfire->GetWidth( ) / 2 ) );
	emitter->SetPosY( newfire->GetPosY( ) - ( newfire->GetHeight( ) / 2 ) );

	emitter->SetVelX( newfire->GetVelX( ) );
	emitter->SetVelY( newfire->GetVelY( ) );

	emitter->GetParticle( )->SetPosX( newfire->GetPosX( ) - ( newfire->GetWidth( )  ) );
	emitter->GetParticle( )->SetPosY( newfire->GetPosY( ) - ( newfire->GetHeight( )  ) );
	emitter->SetLooping( true );

	newfire->SetEmitter(emitter);
	CParticleManager::GetInstance( )->AddEmitter( emitter );
	emitter = NULL;

	newfire->Release();

}

void CSpellFactory::CreateFire(CCharacter* pShooter, int nTier)
{
	CFire* newfire = new CFire();

	newfire->SetElement(OBJ_FIRE);

	newfire->ShotBy(true);
	switch(nTier)
	{
	case 1: // First Tier... Basic Spell
		{
			newfire->SetPosX(pShooter->GetPosX());
			newfire->SetPosY(pShooter->GetPosY() + pShooter->GetHeight() * .25f);

			if(newfire->PlayerShot() && ((CPlayer*)pShooter)->GetReticle())
			{
				CBase* tempRet = ((CPlayer*)pShooter)->GetReticle();

				float speedx = (tempRet->GetPosX() - pShooter->GetPosX()) / 100;
				float speedy = (tempRet->GetPosY() - pShooter->GetPosY()) / 100;

				newfire->SetVelX(250 * speedx);
				newfire->SetVelY(250 * speedy);
			}
			else
			{
				DIRECTION wheretoshoot = pShooter->GetDirection();
				switch(wheretoshoot)
				{
				case 0:
					{
						newfire->SetVelX(-250);
						newfire->SetVelY(0);
						break;
					}
				case 1:
					{
						newfire->SetVelX(250);
						newfire->SetVelY(0);
						break;
					}
				case 2:
					{
						newfire->SetVelX(0);
						newfire->SetVelY(-250);
						break;
					}
				case 3:
					{
						newfire->SetVelX(0);
						newfire->SetVelY(250);
						break;
					}
				case 4:
					{
						newfire->SetVelX(-175);
						newfire->SetVelY(-175);
						break;
					}
				case 5:
					{
						newfire->SetVelX(-175);
						newfire->SetVelY(175);
						break;
					}
				case 6:
					{
						newfire->SetVelX(175);
						newfire->SetVelY(-175);
						break;
					}
				case 7:
					{
						newfire->SetVelX(175);
						newfire->SetVelY(175);
						break;
					}
				}
			}

			newfire->SetDamage(15 + (3 * m_nFireLVL));
			newfire->SetDOT(3 + (1 * (m_nFireLVL>>1)));
			newfire->SetLifespan(5.0f);
			newfire->SetActive(true);
			newfire->SetTier(nTier);

			newfire->SetWidth(32);
			newfire->SetHeight(16);

			break;
		}
	case 2:
		{
			//Lavaflowz.
			newfire->SetPosX(pShooter->GetPosX() + ((int)pShooter->GetWidth() >> 1) );
			newfire->SetPosY(pShooter->GetPosY() + pShooter->GetHeight());
			newfire->SetVelX(100);
			newfire->SetVelY(50);
			newfire->SetDamage(25 + (30 * m_nFireLVL));
			newfire->SetDOT(3);
			newfire->SetLifespan(7.5f);
			newfire->SetTier(nTier);
			newfire->SetWidth(32);
			newfire->SetHeight(16);
			newfire->SetEmitter(NULL);

			CFire* secondfire = new CFire();
			*secondfire = *newfire;
			secondfire->SetVelX(-secondfire->GetVelX());
			Corona_ObjectManager::GetInstance()->AddObject(secondfire);

			CEmitter *emitter;
			emitter = m_pEF->CreateEmitter( "firespell" );
			emitter->SetPosX( secondfire->GetPosX( ) - ( secondfire->GetWidth( ) / 2 ) );
			emitter->SetPosY( secondfire->GetPosY( ) - ( secondfire->GetHeight( ) / 2 ) );

			emitter->SetVelX( secondfire->GetVelX( ) );
			emitter->SetVelY( secondfire->GetVelY( ) );

			emitter->GetParticle( )->SetPosX( secondfire->GetPosX( ) - ( secondfire->GetWidth( )  ) );
			emitter->GetParticle( )->SetPosY( secondfire->GetPosY( ) - ( secondfire->GetHeight( )  ) );
			emitter->SetLooping( true );

			secondfire->SetEmitter(emitter);
			CParticleManager::GetInstance( )->AddEmitter( emitter );
			emitter = NULL;

			secondfire->Release();
			break;
		}

	case 3:
		{
			newfire->SetPosX( CCamera::GetCamera()->GetXOffset() + 1 );
			newfire->SetPosY( CCamera::GetCamera()->GetYOffset() + 1);
			newfire->SetVelX(750);
			newfire->SetVelY(50);
			newfire->SetDamage(50 + (30 * m_nFireLVL));
			newfire->SetDOT(3);
			newfire->SetLifespan(15.0f);
			newfire->SetTier(nTier);
			newfire->SetWidth(32);
			newfire->SetHeight(16);
			newfire->SetEmitter(NULL);

			for(int jay = 0; jay < 5; ++jay)
			{
				for(int index = 0; index < m_nFireLVL * 25 + 50; ++index)
				{
					CFire*  secondfire = new CFire();
					*secondfire = *newfire;

					//secondfire->SetVelX(newfire->GetVelX() * (index + 1 * .25f));
					//secondfire->SetPosY(newfire->GetPosY() + index * 32);

					secondfire->SetVelY(newfire->GetVelY() * index + 32);
					Corona_ObjectManager::GetInstance()->AddObject(secondfire);

					CEmitter *emitter;
					emitter = m_pEF->CreateEmitter( "firespell" );
					emitter->SetPosX( secondfire->GetPosX( ) - ( secondfire->GetWidth( ) / 2 ) );
					emitter->SetPosY( secondfire->GetPosY( ) - ( secondfire->GetHeight( ) / 2 ) );

					emitter->SetVelX( secondfire->GetVelX( ) );
					emitter->SetVelY( secondfire->GetVelY( ) );

					emitter->GetParticle( )->SetPosX( secondfire->GetPosX( ) - ( secondfire->GetWidth( )  ) );
					emitter->GetParticle( )->SetPosY( secondfire->GetPosY( ) - ( secondfire->GetHeight( )  ) );
					emitter->SetLooping( true );

					secondfire->SetEmitter(emitter);
					CParticleManager::GetInstance( )->AddEmitter( emitter );
					emitter = NULL;

					secondfire->Release();
				}
			}
		}
	}

	Corona_ObjectManager::GetInstance()->AddObject(newfire);

	CEmitter *emitter;
	emitter = m_pEF->CreateEmitter( "firespell" );
	emitter->SetPosX( newfire->GetPosX( ) - ( newfire->GetWidth( ) / 2 ) );
	emitter->SetPosY( newfire->GetPosY( ) - ( newfire->GetHeight( ) / 2 ) );

	emitter->SetVelX( newfire->GetVelX( ) );
	emitter->SetVelY( newfire->GetVelY( ) );

	emitter->GetParticle( )->SetPosX( newfire->GetPosX( ) - ( newfire->GetWidth( )  ) );
	emitter->GetParticle( )->SetPosY( newfire->GetPosY( ) - ( newfire->GetHeight( )  ) );
	emitter->SetLooping( true );

	newfire->SetEmitter(emitter);
	CParticleManager::GetInstance( )->AddEmitter( emitter );
	emitter = NULL;

	newfire->Release();
}

void IceTierTwo( CBase* _enemy, CBase* _spell )
{ /* TODO :: Tier Two. */ }

void IceTierThree( CBase* _enemy, CBase* _spell )
{ _enemy->SetVelX( 0.0f ); _enemy->SetVelY( 0.0f ); }

void CSpellFactory::CreateIce(CCharacter* pShooter, int nTier)
{
	CIce* newice = new CIce( );
	newice->SetElement( OBJ_ICE );

	switch( nTier )
	{
	case 1: // First Tier... Basic Spell
		{
			newice->SetPosX( pShooter->GetPosX( ) );
			newice->SetPosY( pShooter->GetPosY( ) + pShooter->GetHeight( ) * .25f );

			if( pShooter->GetType( ) == OBJ_PLAYER && ( ( CPlayer* )pShooter )->GetReticle( ) )
			{
				CBase* tempRet = ( ( CPlayer* )pShooter )->GetReticle( );

				float speedx = ( tempRet->GetPosX( ) - pShooter->GetPosX( ) ) / 100;
				float speedy = ( tempRet->GetPosY( ) - pShooter->GetPosY( ) ) / 100;

				newice->SetVelX( 250 * speedx );
				newice->SetVelY( 250 * speedy );
			}
			else
			{
				DIRECTION wheretoshoot = pShooter->GetDirection( );
				switch( wheretoshoot )
				{
				case 0:
					{
						newice->SetVelX( -150 );
						newice->SetVelY( 0 );
						break;
					}
				case 1:
					{
						newice->SetVelX( 150 );
						newice->SetVelY( 0 );
						break;
					}
				case 2:
					{
						newice->SetVelX( 0 );
						newice->SetVelY( -150 );
						break;
					}
				case 3:
					{
						newice->SetVelX( 0 );
						newice->SetVelY( 150 );
						break;
					}
				case 4:
					{
						newice->SetVelX( -75 );
						newice->SetVelY( -75 );
						break;
					}
				case 5:
					{
						newice->SetVelX( -75 );
						newice->SetVelY( 75 );
						break;
					}
				case 6:
					{
						newice->SetVelX( 75 );
						newice->SetVelY( -75 );
						break;
					}
				case 7:
					{
						newice->SetVelX( 75 );
						newice->SetVelY( 75 );
						break;
					}
				}
			}

			newice->SetDamage( 9 + 2 * m_nIceLVL );
			newice->SetSlow( 10.0f + 2.0f * m_nIceLVL );
			newice->SetWidth( 32 );
			newice->SetHeight( 16 );
			newice->SetActive( true );
			newice->SetLifespan( 5.0f );

			if( pShooter->GetType( ) == OBJ_PLAYER )
				newice->ShotBy( true );
			else newice->ShotBy( false ); 

			newice->SetTier( nTier );
			break;
		}
	case 2:
		{
			break;
		}
	case 3:
		{
			Corona_ObjectManager::GetInstance( )->AuxFunction
				( &IceTierThree, OBJ_ENEMY, true, NULL );
			break;
		}
	}	

	Corona_ObjectManager::GetInstance( )->AddObject( newice );
	
	CEmitter *emitter;
	emitter = m_pEF->CreateEmitter( "icespell" );
	emitter->SetPosX( newice->GetPosX( ) - ( newice->GetWidth( ) / 2 ) );
	emitter->SetPosY( newice->GetPosY( ) - ( newice->GetHeight( ) / 2 ) );

	emitter->SetVelX( newice->GetVelX( ) );
	emitter->SetVelY( newice->GetVelY( ) );

	emitter->GetParticle( )->SetPosX( newice->GetPosX( ) - ( newice->GetWidth( ) ) );
	emitter->GetParticle( )->SetPosY( newice->GetPosY( ) - ( newice->GetHeight( ) ) );
	emitter->SetLooping( true );

	newice->SetEmitter( emitter );
	CParticleManager::GetInstance( )->AddEmitter( emitter );
	emitter = NULL;

	newice->Release( );
}

void CSpellFactory::CreateWind(CCharacter* pShooter, int nTier)
{
	CWind* newwind = new CWind();
	if(pShooter->GetType() == OBJ_PLAYER)
	{
		switch(nTier)
		{
		case 1: // First Tier... Basic wind Spell
			{
				
				newwind->SetPosX(pShooter->GetPosX());
				newwind->SetPosY(pShooter->GetPosY() + pShooter->GetHeight() * .25f);
				if(pShooter->GetType() == OBJ_PLAYER && ((CPlayer*)pShooter)->GetReticle())
				{
					CBase* tempRet = ((CPlayer*)pShooter)->GetReticle();

				float speedx = (tempRet->GetPosX() - pShooter->GetPosX()) / 100;
				float speedy = (tempRet->GetPosY() - pShooter->GetPosY()) / 100;

				newwind->SetVelX(250 * speedx);
				newwind->SetVelY(250 * speedy);


				}
				else
				{
					DIRECTION wheretoshoot = pShooter->GetDirection();
					switch(wheretoshoot)
					{
					case 0:
						{
							newwind->SetVelX(-150);
							newwind->SetVelY(0);
							break;
						}
					case 1:
						{
							newwind->SetVelX(150);
							newwind->SetVelY(0);
							break;
						}
					case 2:
						{
							newwind->SetVelX(0);
							newwind->SetVelY(-150);
							break;
						}
					case 3:
						{
							newwind->SetVelX(0);
							newwind->SetVelY(150);
							break;
						}
					case 4:
						{
							newwind->SetVelX(-75);
							newwind->SetVelY(-75);
							break;
						}
					case 5:
						{
							newwind->SetVelX(-75);
							newwind->SetVelY(75);
							break;
						}
					case 6:
						{
							newwind->SetVelX(75);
							newwind->SetVelY(-75);
							break;
						}
					case 7:
						{
							newwind->SetVelX(75);
							newwind->SetVelY(75);
							break;
						}
					}
				}

				newwind->SetActive(true);
				newwind->SetElement(OBJ_WIND);
				newwind->SetDamage(9 + m_nWindLVL);
				newwind->SetPushBack(10.0f + 5 *m_nWindLVL);
				newwind->SetLifespan(5.0f);
				newwind->SetTier(nTier);
				newwind->SetWidth(32);
				newwind->SetHeight(16);

				if(pShooter->GetType() == OBJ_PLAYER)
				{
					newwind->ShotBy(true);
				}
				else
				{
					newwind->ShotBy(false);
				}
				Corona_ObjectManager::GetInstance()->AddObject(newwind);
				newwind->Release();

				break;
			}
		case 2:
			{
				newwind->SetPosX(pShooter->GetPosX());
				newwind->SetPosY(pShooter->GetPosY() + pShooter->GetHeight() * .25f);
				newwind->SetCaster((CPlayer*)pShooter);
				if(pShooter->GetType() == OBJ_PLAYER && ((CPlayer*)pShooter)->GetReticle())
				{
					CBase* tempRet = ((CPlayer*)pShooter)->GetReticle();

					float speedx = (tempRet->GetPosX() - pShooter->GetPosX()) / 100;
					float speedy = (tempRet->GetPosY() - pShooter->GetPosY()) / 100;

					newwind->SetVelX(250 * speedx);
					newwind->SetVelY(250 * speedy);


				}
				else
				{
					DIRECTION wheretoshoot = pShooter->GetDirection();
					switch(wheretoshoot)
					{
					case 0:
						{
							newwind->SetVelX(-150);
							newwind->SetVelY(0);
							break;
						}
					case 1:
						{
							newwind->SetVelX(150);
							newwind->SetVelY(0);
							break;
						}
					case 2:
						{
							newwind->SetVelX(0);
							newwind->SetVelY(-150);
							break;
						}
					case 3:
						{
							newwind->SetVelX(0);
							newwind->SetVelY(150);
							break;
						}
					case 4:
						{
							newwind->SetVelX(-75);
							newwind->SetVelY(-75);
							break;
						}
					case 5:
						{
							newwind->SetVelX(-75);
							newwind->SetVelY(75);
							break;
						}
					case 6:
						{
							newwind->SetVelX(75);
							newwind->SetVelY(-75);
							break;
						}
					case 7:
						{
							newwind->SetVelX(75);
							newwind->SetVelY(75);
							break;
						}
					}
				}

				newwind->SetActive(true);
				newwind->SetElement(OBJ_WIND);
				newwind->SetDamage(9 + m_nWindLVL);
				newwind->SetPushBack(10.0f + 5 *m_nWindLVL);
				newwind->SetLifespan(.25f);
				newwind->SetTier(nTier);
				newwind->SetWidth(16);
				newwind->SetHeight(16);
				newwind->ShotBy(true);
				Corona_ObjectManager::GetInstance()->AddObject(newwind);
				newwind->Release();

				break;
			}
		case 3:
			{
				newwind->SetPosX(pShooter->GetPosX());
				newwind->SetPosY(pShooter->GetPosY());
				newwind->SetCaster((CPlayer*)pShooter);
				((CPlayer*)pShooter)->SetShielded(true);
				newwind->SetActive(true);
				newwind->SetElement(OBJ_WIND);
				newwind->SetDamage(0);
				newwind->SetPushBack(10.0f + 5 *m_nWindLVL);
				newwind->SetLifespan(30.0f);
				newwind->SetTier(nTier);
				newwind->SetWidth(pShooter->GetWidth());
				newwind->SetHeight(pShooter->GetHeight());
				newwind->ShotBy(true);
				Corona_ObjectManager::GetInstance()->AddObject(newwind);
				newwind->Release();
				break;
			}
		}
	}
	CEmitter *emitter;
	emitter = m_pEF->CreateEmitter( "windSpell" );
	emitter->SetPosX( newwind->GetPosX( ) - ( newwind->GetWidth( ) / 2 ) );
	emitter->SetPosY( newwind->GetPosY( ) - ( newwind->GetHeight( ) / 2 ) );

	emitter->SetVelX( newwind->GetVelX( ) );
	emitter->SetVelY( newwind->GetVelY( ) );

	emitter->GetParticle( )->SetPosX( newwind->GetPosX( ) - ( newwind->GetWidth( )  ) );
	emitter->GetParticle( )->SetPosY( newwind->GetPosY( ) - ( newwind->GetHeight( )  ) );
	emitter->SetLooping( true );

	newwind->SetEmitter(emitter);
	CParticleManager::GetInstance( )->AddEmitter( emitter );
	emitter = NULL;
}
