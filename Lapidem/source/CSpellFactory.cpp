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
#include "stdheaders.h"
#include "CCharacter.h"

#include "CFire.h"
#include "CIce.h"
#include "CEarth.h"
#include "CWind.h"

#include "CGrenade.h"
#include "CIceSpear.h"
#include "CGiantFireBall.h"
#include "CIceCube.h"

#include "Lapidem_Math.h"

CSpellFactory* CSpellFactory::m_pSF = NULL;

CSpellFactory::CSpellFactory()
{
	m_nEarthLVL  = 0;
	m_nWindLVL   = 0;
	m_nFireLVL   = 0;
	m_nIceLVL    = 0;

	m_nEarthXP   = 0;
	m_nWindXP    = 0;
	m_nFireXP    = 0;
	m_nIceXP     = 0;
}

CSpellFactory::~CSpellFactory()
{
}

void CSpellFactory::AddWindXP(int nXP)
{
	m_nWindXP += nXP;
	if(m_nWindXP> (35*(m_nWindLVL + (m_nWindLVL>>1))))
	{
		m_nWindXP -=(int)(35*(m_nWindLVL + (m_nWindLVL>>1)));
		m_nWindLVL++;
	}
}
void CSpellFactory::AddEarthXP(int nXP)
{
	m_nEarthXP += nXP;
	if(m_nEarthXP > (25+(20* m_nEarthLVL)))
	{
		m_nEarthXP -= (int)(25+(20* m_nEarthLVL));
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

					if(newearth->GetVelX() > 100)
						newearth->SetVelX(100);
					else if(newearth->GetVelX() < -100 )
						newearth->SetVelX(-100);

					if(newearth->GetVelY() > 100)
						newearth->SetVelY(100);
					else if(newearth->GetVelX() < -100)
						newearth->SetVelY(-100);

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

				//////////////////////////
				// Bug #13 Bug Fix
				//
				// Sam Mathis
				//////////////////////////
				CTerrainBase* pTile = (CTerrainBase*)CGameplayState::GetInstance()->GetLevel()->GetTile( (int)newearth->GetPosX(), (int)newearth->GetPosY() );
				if(pTile)
				{
					if(pTile->GetTypeTerrain() != T_EMPTY 
						&& newearth->GetPosX() > pTile->GetPosX() - 3 && newearth->GetPosX() < pTile->GetPosX() + 7 
						&& newearth->GetPosY() > pTile->GetPosY() - 3 && newearth->GetPosY() < pTile->GetPosY() + 16 )
					{
						newearth->Release();
						return;
					}
				}
				else
				{
					newearth->Release();
					return;
				}

				//////////////////////////
				// End Bug Fix
				//
				// 
				//////////////////////////

				newearth->SetDamage(10 + 4 * m_nEarthLVL);
				newearth->SetLifespan(10.0f + 1.5f * m_nEarthLVL);
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
				newearth->SetImage(CSGD_TextureManager::GetInstance()->LoadTexture
					("resource/graphics/Lapidem_EarthPillar.png"));
				Corona_ObjectManager::GetInstance()->AddObject(newearth);
				newearth->Release();
				break;
			}
		case 3:
			{
				CEarth* newearth = new CEarth();
				newearth->SetHeight(CGame::GetInstance()->GetScreenHeight());
				newearth->SetWidth(CGame::GetInstance()->GetScreenWidth());	

				if(pShooter->GetType() == OBJ_PLAYER && ((CPlayer*)pShooter)->GetReticle())
				{
					int retposition = int(((CPlayer*)pShooter)->GetReticle()->GetPosX());

					if(pShooter->GetVelX() < retposition)
					{
						newearth->SetPosX(CCamera::GetCamera()->GetWidth() + newearth->GetWidth());
						newearth->SetVelX(-650.0f);
					}
					else
					{
						newearth->SetPosX(CCamera::GetCamera()->GetXOffset() - newearth->GetWidth());
						newearth->SetVelX(650.0f);
						newearth->SetXScale(-1.0f);	
					}
				}
				else
				{
					int direction = pShooter->GetDirection();
					if(direction == LEFT || direction == LEFT_DOWN || direction == LEFT_UP || direction == UP)
					{
						newearth->SetPosX(CCamera::GetCamera()->GetWidth() + newearth->GetWidth());
						newearth->SetVelX(-650.0f);
					}
					else
					{
						newearth->SetPosX(CCamera::GetCamera()->GetXOffset() - newearth->GetWidth());
						newearth->SetVelX(650.0f);
						newearth->SetXScale(-1.0f);		
					}
				}
				newearth->SetPosY(CCamera::GetCamera()->GetYOffset());
				newearth->SetVelY(0.0f);
				newearth->SetDamage(200);
				newearth->SetLifespan(400);
				newearth->SetActive(true);
				newearth->SetTier(3);
				newearth->ShotBy(true);	
				newearth->SetElement(OBJ_EARTH);
				newearth->SetImage(CSGD_TextureManager::GetInstance()->LoadTexture
					("resource/graphics/Lapidem_EarthHand.png", D3DCOLOR_XRGB(255,255,255)));
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
			newearth->SetPosX(CGameplayState::GetInstance()->GetPlayerOne()->GetPosX() + 
				(CGameplayState::GetInstance()->GetPlayerOne()->GetWidth()>>1));
			newearth->SetPosY(CGameplayState::GetInstance()->GetPlayerOne()->GetPosY() - 100);
		}
		else if(nTier ==2)
		{
			newearth->SetPosX(CGameplayState::GetInstance()->GetPlayerTwo()->GetPosX() + 
				(CGameplayState::GetInstance()->GetPlayerTwo()->GetWidth()>>1));
			newearth->SetPosY(CGameplayState::GetInstance()->GetPlayerTwo()->GetPosY() - 100);
		}

		//////////////////////////
		// Bug #13 Bug Fix
		//
		// Sam Mathis
		//////////////////////////

		CTerrainBase* pTile = (CTerrainBase*)CGameplayState::GetInstance()->GetLevel()->GetTile( (int)newearth->GetPosX(), (int)newearth->GetPosY() );
		if(pTile)
		{
			if(pTile->GetTypeTerrain() == T_ROCK)
			{
				newearth->Release();
				return;
			}
		}
		else
		{
			newearth->Release();
			return;
		}

		//////////////////////////
		// End Bug Fix
		//
		// 
		//////////////////////////


		newearth->SetVelX(0);
		newearth->SetVelY(100);
		newearth->SetDamage(10);
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

	newfire->SetPosX(pShooter->GetPosX() + pShooter->GetWidth() * .5f);
	newfire->SetPosY(pShooter->GetPosY() - 2);


	//////////////////////////////
	// Bug #6 Fix
	//
	// Sam Mathis
	//////////////////////////////
	//float SpeedX =  ( (pTarget->GetPosX() + pTarget->GetWidth()  * .5f) - newfire->GetPosX() ) / 100;
	//float SpeedY =  ( (pTarget->GetPosY() + pTarget->GetHeight() * .5f) - newfire->GetPosY() ) / 100;
	//newfire->SetVelX( SpeedX * 250 );
	//newfire->SetVelY( SpeedY * 250 );

	float xComp = (pTarget->GetPosX() + pTarget->GetWidth()  * .5f) - newfire->GetPosX();
	float yComp = (pTarget->GetPosY() + pTarget->GetHeight() * .5f) - newfire->GetPosY();

	newfire->SetVelX( xComp / sqrt(( xComp * xComp + yComp * yComp )) * 200);
	newfire->SetVelY( yComp / sqrt(( xComp * xComp + yComp * yComp )) * 200);



	///////////////////////////
	// End Bug Fix
	//
	//////////////////////////

	newfire->SetDamage(10 * CGameplayState::GetInstance()->GetDifficulty());
	newfire->SetDOT(0);
	newfire->SetLifespan(5.0f);
	newfire->SetActive(true);
	newfire->SetTier(1);

	newfire->SetWidth(32);
	newfire->SetHeight(16);

	Corona_ObjectManager::GetInstance()->AddObject(newfire);

	CEmitter* hahaiworknow = CParticleManager::GetInstance()->LoadEmitter("resource/data/fireSpell.lapipt",0,0);
	newfire->SetEmitter(hahaiworknow);
	CParticleManager::GetInstance()->AddEmitter(newfire->GetEmitter());

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

				//////////////////
				// Bug #6 Fix
				// Sam Mathis
				//////////////////
				//float speedx = (tempRet->GetPosX() - pShooter->GetPosX()) / 100;
				//float speedy = (tempRet->GetPosY() - pShooter->GetPosY()) / 100;

				//newfire->SetVelX(200 * speedx);
				//newfire->SetVelY(200 * speedy);


				float xComp = (tempRet->GetPosX() + tempRet->GetWidth()  * .5f) - newfire->GetPosX();
				float yComp = (tempRet->GetPosY() + tempRet->GetHeight() * .5f) - newfire->GetPosY();

				newfire->SetVelX( xComp / sqrt(( xComp * xComp + yComp * yComp )) * 250);
				newfire->SetVelY( yComp / sqrt(( xComp * xComp + yComp * yComp )) * 250);

				if(newfire->GetVelX() > 0)
					newfire->SetPosX(pShooter->GetPosX() + pShooter->GetWidth());


				/////////////////
				// End Bug Fix
				//
				////////////////



			}
			else
			{
				DIRECTION wheretoshoot = pShooter->GetDirection();

				if( wheretoshoot == RIGHT || wheretoshoot == RIGHT_UP || wheretoshoot == RIGHT_DOWN)
					newfire->SetPosX(pShooter->GetPosX() + pShooter->GetWidth());
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
						newfire->SetVelX(-200);
						newfire->SetVelY(-200);
						break;
					}
				case 5:
					{
						newfire->SetVelX(-200);
						newfire->SetVelY(200);
						break;
					}
				case 6:
					{
						newfire->SetVelX(200);
						newfire->SetVelY(-200);
						break;
					}
				case 7:
					{
						newfire->SetVelX(200);
						newfire->SetVelY(200);
						break;
					}
				}
			}

			newfire->SetDamage(12 + (3 * m_nFireLVL));
			newfire->SetDOT(3 + (1 * (m_nFireLVL>>1)));
			newfire->SetLifespan(7.5f);
			newfire->SetActive(true);
			newfire->SetTier(nTier);

			newfire->SetWidth(32);
			newfire->SetHeight(16);

			CEmitter* hahaiworknow = CParticleManager::GetInstance()->LoadEmitter("resource/data/fireSpell.lapipt",0,0);
			newfire->SetEmitter(hahaiworknow);
			CParticleManager::GetInstance()->AddEmitter(newfire->GetEmitter());

			break;
		}
	case 2:
		{
			//Lavaflowz.
			newfire->SetPosX(pShooter->GetPosX() + ((int)pShooter->GetWidth() >> 1) );
			newfire->SetPosY(pShooter->GetPosY() + pShooter->GetHeight() - 32);
			newfire->SetVelX(100);
			newfire->SetVelY(50);
			newfire->SetDamage(2 + (2 * m_nFireLVL));
			newfire->SetDOT(3);
			newfire->SetLifespan(10.f);
			newfire->SetTier(nTier);
			newfire->SetWidth(32);
			newfire->SetHeight(30);
			newfire->SetEmitter(NULL);

			CFire* secondfire = new CFire();
			*secondfire = *newfire;
			secondfire->SetVelX(-secondfire->GetVelX());
			Corona_ObjectManager::GetInstance()->AddObject(secondfire);

			CEmitter* hahaiworknow = CParticleManager::GetInstance()->LoadEmitter("resource/data/lava.lapipt",0,0);
			secondfire->SetEmitter(hahaiworknow);
			CParticleManager::GetInstance()->AddEmitter(secondfire->GetEmitter());

			hahaiworknow = CParticleManager::GetInstance()->LoadEmitter("resource/data/lava.lapipt",0,0);
			newfire->SetEmitter(hahaiworknow);
			CParticleManager::GetInstance()->AddEmitter(newfire->GetEmitter());

			secondfire->Release();
			break;
		}

	case 3:
		{
			newfire->SetPosX( CCamera::GetCamera()->GetXOffset() + 5 );
			newfire->SetPosY( CCamera::GetCamera()->GetYOffset() + 5);
			newfire->SetVelX(750);
			newfire->SetVelY(50);
			newfire->SetDamage(5 + (2 * m_nFireLVL));
			newfire->SetDOT(3);
			newfire->SetLifespan(10.0f);
			newfire->SetTier(nTier);
			newfire->SetWidth(32);
			newfire->SetHeight(16);
			newfire->SetEmitter(NULL);

			//for(int jay = 0; jay < 5; ++jay)
			//{
			for(int index = 0; index < m_nFireLVL * 25 + 50; ++index)
			{
				CFire*  secondfire = new CFire();
				*secondfire = *newfire;

				secondfire->SetVelY(newfire->GetVelY() * index + 32);
				Corona_ObjectManager::GetInstance()->AddObject(secondfire);

				CEmitter* hahaiworknow = CParticleManager::GetInstance()->LoadEmitter("resource/data/lava.lapipt",0,0);
				secondfire->SetEmitter(hahaiworknow);
				CParticleManager::GetInstance()->AddEmitter(secondfire->GetEmitter());
				secondfire->Release();
			}
			//}


		}
	}

	Corona_ObjectManager::GetInstance()->AddObject(newfire);


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

				////////////////
				// Bug #6 Fix
				////////////////


				//float speedx = ( tempRet->GetPosX( ) - pShooter->GetPosX( ) ) / 100;
				//float speedy = ( tempRet->GetPosY( ) - pShooter->GetPosY( ) ) / 100;

				//newice->SetVelX( 150 * speedx );
				//newice->SetVelY( 150 * speedy );

				float xComp = (tempRet->GetPosX() + tempRet->GetWidth()  * .5f) - newice->GetPosX();
				float yComp = (tempRet->GetPosY() + tempRet->GetHeight() * .5f) - newice->GetPosY();

				newice->SetVelX( xComp / sqrt(( xComp * xComp + yComp * yComp )) * 200);
				newice->SetVelY( yComp / sqrt(( xComp * xComp + yComp * yComp )) * 200);

				if(newice->GetVelX() > 0)
					newice->SetPosX( pShooter->GetPosX() + pShooter->GetWidth() );

				/////////////
				//End Bug Fix
				/////////////
			}
			else
			{
				DIRECTION wheretoshoot = pShooter->GetDirection( );
				if( wheretoshoot == RIGHT || wheretoshoot == RIGHT_UP || wheretoshoot == RIGHT_DOWN)
					newice->SetPosX(pShooter->GetPosX() + pShooter->GetWidth());
				switch( wheretoshoot )
				{
				case 0:
					{
						newice->SetVelX( -200 );
						newice->SetVelY( 0 );
						break;
					}
				case 1:
					{
						newice->SetVelX( 200 );
						newice->SetVelY( 0 );
						break;
					}
				case 2:
					{
						newice->SetVelX( 0 );
						newice->SetVelY( -200 );
						break;
					}
				case 3:
					{
						newice->SetVelX( 0 );
						newice->SetVelY( 200 );
						break;
					}
				case 4:
					{
						newice->SetVelX( -125 );
						newice->SetVelY( -125 );
						break;
					}
				case 5:
					{
						newice->SetVelX( -125 );
						newice->SetVelY( 125 );
						break;
					}
				case 6:
					{
						newice->SetVelX( 125 );
						newice->SetVelY( -125 );
						break;
					}
				case 7:
					{
						newice->SetVelX( 125 );
						newice->SetVelY( 125 );
						break;
					}
				}
			}

			newice->SetDamage( 12 + 2 * m_nIceLVL );
			newice->SetSlow( 10.0f + 2.0f * m_nIceLVL );
			newice->SetWidth( 32 );
			newice->SetHeight( 16 );
			newice->SetActive( true );
			newice->SetLifespan( 7.5f );
			newice->ShotBy( true );

			newice->SetTier( nTier );

			Corona_ObjectManager::GetInstance( )->AddObject( newice );

			CEmitter* hahaiworknow = CParticleManager::GetInstance()->LoadEmitter("resource/data/iceSpell.lapipt",0,0);
			newice->SetEmitter(hahaiworknow);
			CParticleManager::GetInstance()->AddEmitter(newice->GetEmitter());


			break;
		}
	case 2:
		{
			newice->SetPosX( pShooter->GetPosX( ) + pShooter->GetWidth() - 15);
			newice->SetPosY( pShooter->GetPosY( ) + pShooter->GetHeight( ) * .25f);
			newice->SetVelX( 50 );
			newice->SetVelY( 0 );
			newice->SetDamage( 5 + 2 * m_nIceLVL );
			newice->SetSlow( 75.0f + 2.0f * m_nIceLVL );
			newice->SetWidth( 16);
			newice->SetHeight( 8 );
			newice->SetActive( true );
			newice->SetLifespan( 2.f );
			newice->ShotBy(true);
			newice->SetEmitter( NULL );
			newice->SetTier( nTier );
			newice->SetElement(OBJ_ICE);

			CEmitter* hahaiworknow = CParticleManager::GetInstance()->LoadEmitter("resource/data/iceSpellT2.lapipt",0,0);
			CEmitter* hahaialsowork = CParticleManager::GetInstance()->LoadEmitter("resource/data/iceSpellT2two.lapipt",0,0);

			CIce * secondice = new CIce();
			*secondice = *newice;
			secondice->SetVelX( -50 );
			secondice->SetPosX( pShooter->GetPosX() );

			newice->SetEmitter(hahaiworknow);
			secondice->SetEmitter(hahaialsowork);
			CParticleManager::GetInstance()->AddEmitter(newice->GetEmitter());
			CParticleManager::GetInstance()->AddEmitter(secondice->GetEmitter());

			Corona_ObjectManager::GetInstance()->AddObject(newice);
			Corona_ObjectManager::GetInstance()->AddObject(secondice);
			secondice->Release();


			break;
		}
	case 3:
		{
			Corona_ObjectManager::GetInstance( )->AuxFunction
				( &IceTierThree, OBJ_ENEMY, true, NULL );
			break;
		}
	}	


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


					/////////////////
					// Bug Fix 6
					////////////////
					//float speedx = (tempRet->GetPosX() - pShooter->GetPosX()) / 100;
					//float speedy = (tempRet->GetPosY() - pShooter->GetPosY()) / 100;

					//newwind->SetVelX(250 * speedx);
					//newwind->SetVelY(250 * speedy);


					float xComp = (tempRet->GetPosX() + tempRet->GetWidth()  * .5f) - newwind->GetPosX();
					float yComp = (tempRet->GetPosY() + tempRet->GetHeight() * .5f) - newwind->GetPosY();

					newwind->SetVelX( xComp / sqrt(( xComp * xComp + yComp * yComp )) * 300);
					newwind->SetVelY( yComp / sqrt(( xComp * xComp + yComp * yComp )) * 300);

					if(newwind->GetVelX() > 0)
						newwind->SetPosX(pShooter->GetPosX() + pShooter->GetWidth());


					/////////////////
					// End Bug Fix
					/////////////////

				}
				else
				{
					DIRECTION wheretoshoot = pShooter->GetDirection();
					if( wheretoshoot == RIGHT || wheretoshoot == RIGHT_UP || wheretoshoot == RIGHT_DOWN)
						newwind->SetPosX(pShooter->GetPosX() + pShooter->GetWidth());

					switch(wheretoshoot)
					{
					case 0:
						{
							newwind->SetVelX(-300);
							newwind->SetVelY(0);
							break;
						}
					case 1:
						{
							newwind->SetVelX(300);
							newwind->SetVelY(0);
							break;
						}
					case 2:
						{
							newwind->SetVelX(0);
							newwind->SetVelY(-300);
							break;
						}
					case 3:
						{

							newwind->SetVelX(0);
							newwind->SetVelY(300);
							break;

						}
					case 4:
						{
							newwind->SetVelX(-225);
							newwind->SetVelY(-225);
							break;
						}
					case 5:
						{
							newwind->SetVelX(-225);
							newwind->SetVelY(225);
							break;
						}
					case 6:
						{
							newwind->SetVelX(225);
							newwind->SetVelY(-225);
							break;
						}
					case 7:
						{
							newwind->SetVelX(225);
							newwind->SetVelY(225);
							break;
						}
					}

				}

				newwind->SetActive(true);
				newwind->SetElement(OBJ_WIND);
				newwind->SetDamage(10 + m_nWindLVL);
				newwind->SetPushBack(10.0f + 5 * m_nWindLVL);
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


				CEmitter* hahaiworknow = CParticleManager::GetInstance()->LoadEmitter("resource/data/windSpell.lapipt",0,0);
				newwind->SetEmitter(hahaiworknow);
				CParticleManager::GetInstance()->AddEmitter(newwind->GetEmitter());

				break;
			}
		case 2:
			{
				newwind->SetPosX(pShooter->GetPosX());
				newwind->SetPosY(pShooter->GetPosY() + pShooter->GetHeight() * .25f);
				newwind->SetCaster((CPlayer*)pShooter);

				///////////////////////////////
				//	Bug Fix # 2
				//Pablo Leon
				//////////////////////////////


				if(pShooter->GetType() == OBJ_PLAYER && ((CPlayer*)pShooter)->GetReticle())
				{
					CBase* tempRet = ((CPlayer*)pShooter)->GetReticle();


					///////////
					//Bug 6 fix't
					///////////

					//float speedx = (tempRet->GetPosX() - pShooter->GetPosX()) / 100;
					//float speedy = (tempRet->GetPosY() - pShooter->GetPosY()) / 100;

					//newwind->SetVelX(250 * speedx);
					//newwind->SetVelY(250 * speedy);

					float xComp = (tempRet->GetPosX() + tempRet->GetWidth()  * .5f) - newwind->GetPosX();
					float yComp = (tempRet->GetPosY() + tempRet->GetHeight() * .5f) - newwind->GetPosY();

					newwind->SetVelX( xComp / sqrt(( xComp * xComp + yComp * yComp )) * 150);
					newwind->SetVelY( yComp / sqrt(( xComp * xComp + yComp * yComp )) * 150);

					//End Fix't


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




				newwind->SetVelX(0);
				newwind->SetVelY(150);

				////////////////
				//end bug fix
				/////////////////
				newwind->SetActive(true);
				newwind->SetElement(OBJ_WIND);
				newwind->SetDamage(10 + m_nWindLVL);
				newwind->SetPushBack(10.0f + 5 *m_nWindLVL);
				newwind->SetLifespan(.4f);
				newwind->SetTier(nTier);
				newwind->SetWidth(16);
				newwind->SetHeight(16);
				newwind->ShotBy(true);
				Corona_ObjectManager::GetInstance()->AddObject(newwind);
				newwind->Release();


				CEmitter* hahaiworknow = CParticleManager::GetInstance()->LoadEmitter("resource/data/aircurent.lapipt",0,0);
				newwind->SetEmitter(hahaiworknow);
				CParticleManager::GetInstance()->AddEmitter(newwind->GetEmitter());

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
				newwind->SetPushBack(5.0f + 5 *m_nWindLVL);
				newwind->SetLifespan(30.0f);
				newwind->SetTier(nTier);
				newwind->SetWidth(pShooter->GetWidth());
				newwind->SetHeight(pShooter->GetHeight());
				newwind->ShotBy(true);
				Corona_ObjectManager::GetInstance()->AddObject(newwind);
				newwind->Release();


				CEmitter* hahaiworknow = CParticleManager::GetInstance()->LoadEmitter("resource/data/windt3.lapipt",0,0);
				newwind->SetEmitter(hahaiworknow);
				CParticleManager::GetInstance()->AddEmitter(newwind->GetEmitter());
				break;
			}
		}
	}

}

void CSpellFactory::CreateGrenade(CSpell* pFire, CSpell* pEarth)
{
	float VelX = (pFire->GetVelX() + pEarth->GetVelX()) * 0.5f;
	float VelY = (pFire->GetVelY() + pEarth->GetVelY()) * 0.5f;

	CGrenade* pSpell = new CGrenade;
	pSpell->SetPosX(pEarth->GetPosX());
	pSpell->SetPosY(pEarth->GetPosY());
	pSpell->SetVelX(VelX);
	pSpell->SetVelY(VelY);

	pSpell->SetTier(1);
	pSpell->SetDamage(20);
	pSpell->SetLifespan(3.0f);
	pSpell->SetHeight(32);
	pSpell->SetWidth(32);
	pSpell->SetImage(pEarth->GetImage());
	pSpell->ShotBy(true);
	CEmitter* hahaiworknow = CParticleManager::GetInstance()->LoadEmitter("resource/data/fireSpell.lapipt",0,0);
	pSpell->SetEmitter(hahaiworknow);
	CParticleManager::GetInstance()->AddEmitter(pSpell->GetEmitter());
	pSpell->SetSound(pFire->GetSound());

	Corona_ObjectManager::GetInstance()->RemoveObject(pEarth);
	pFire->SetActive(false);

	Corona_ObjectManager::GetInstance()->AddObject(pSpell);
	pSpell->Release();
}

void CSpellFactory::CreateGiantFireBall(CSpell* pFire, CSpell* pWind)
{
	float VelX = (pFire->GetVelX() + pWind->GetVelX()) * 0.5f;
	float VelY = (pFire->GetVelY() + pWind->GetVelY()) * 0.5f;

	CGiantFireBall* pSpell = new CGiantFireBall;
	pSpell->SetPosX(pFire->GetPosX());
	pSpell->SetPosY(pFire->GetPosY());
	pSpell->SetVelX(VelX);
	pSpell->SetVelY(VelY);

	pSpell->SetTier(1);
	pSpell->SetDamage(80);
	pSpell->SetLifespan(5.0f);
	pSpell->SetHeight(32);
	pSpell->SetWidth(32);
	pSpell->SetDOT(2);

	pSpell->SetRadiusIncrease((float)(16 + pSpell->GetWidth()));
	pSpell->ShotBy(true);

	CEmitter* hahaiworknow = CParticleManager::GetInstance()->LoadEmitter("resource/data/fireSpell.lapipt",0,0);
	hahaiworknow->SetNumberOfParticles(hahaiworknow->GetNumberOfParticles() + 100);
	hahaiworknow->SetLifeSpan(1.0f);
	pSpell->SetEmitter(hahaiworknow);
	CParticleManager::GetInstance()->AddEmitter(pSpell->GetEmitter());

	pSpell->SetSound(pFire->GetSound());

	Corona_ObjectManager::GetInstance()->RemoveObject(pWind);
	pFire->SetActive(false);

	Corona_ObjectManager::GetInstance()->AddObject(pSpell);
	pSpell->Release();
}

void CSpellFactory::CreateSpear(CSpell* pIce, CSpell* pWind)
{
	float VelX = pIce->GetVelX() + pWind->GetVelX() * 0.2f;
	float VelY = pIce->GetVelY() + pWind->GetVelY() * 0.2f;

	CIceSpear* pSpell = new CIceSpear;
	pSpell->SetPosX(pIce->GetPosX());
	pSpell->SetPosY(pIce->GetPosY());
	pSpell->SetVelX(VelX);
	pSpell->SetVelY(VelY);

	pSpell->SetTier(1);
	pSpell->SetDamage(5);
	pSpell->SetLifespan(5.0f);
	pSpell->SetHeight(32);
	pSpell->SetWidth(64);
	//pSpell->SetImage(CSGD_TextureManager::GetInstance()->LoadTexture("resource\\graphics\\Lapidem_IceSpear.png", D3DCOLOR_XRGB(255, 255, 255)));
	pSpell->ShotBy(true);

	pSpell->SetSound(pIce->GetSound());

	CEmitter* hahaiworknow = CParticleManager::GetInstance()->LoadEmitter("resource/data/icewind.lapipt",0,0);
	pSpell->SetEmitter(hahaiworknow);
	CParticleManager::GetInstance()->AddEmitter(pSpell->GetEmitter());

	Corona_ObjectManager::GetInstance()->RemoveObject(pWind);
	pIce->SetActive(false);

	Corona_ObjectManager::GetInstance()->AddObject(pSpell);
	pSpell->Release();
}

void CSpellFactory::CreateIceCube(CSpell* pIce, CSpell* pEarth)
{
	float VelX = (pIce->GetVelX() + pEarth->GetVelX()) * 0.5f;
	float VelY = (pIce->GetVelY() + pEarth->GetVelY()) * 0.5f;

	CIceCube* pSpell = new CIceCube;
	pSpell->SetPosX(pEarth->GetPosX());
	pSpell->SetPosY(pEarth->GetPosY());
	pSpell->SetVelX(VelX);
	pSpell->SetVelY(VelY);

	pSpell->SetTier(1);
	pSpell->SetDamage(10);
	pSpell->SetLifespan(3.0f);
	pSpell->SetHeight(32);
	pSpell->SetWidth(32);
	pSpell->SetImage(pEarth->GetImage());
	pSpell->ShotBy(true);
	pSpell->SetElement(OBJ_ICE);


	pSpell->SetSound(pIce->GetSound());

	CEmitter* hahaiworknow = CParticleManager::GetInstance()->LoadEmitter("resource/data/iceSpell.lapipt",0,0);
	pSpell->SetEmitter(hahaiworknow);
	CParticleManager::GetInstance()->AddEmitter(pSpell->GetEmitter());

	Corona_ObjectManager::GetInstance()->RemoveObject(pEarth);
	pIce->SetActive(false);

	Corona_ObjectManager::GetInstance()->AddObject(pSpell);
	pSpell->Release();
}

void CSpellFactory::CreateEnemyWind(CCharacter * pShooter, CBase * pTarget)
{
	CWind* newWind = new CWind();
	newWind->SetElement(OBJ_WIND);
	newWind->ShotBy(false);

	newWind->SetPosX(pShooter->GetPosX() + pShooter->GetWidth()  * .5f);
	newWind->SetPosY(pShooter->GetPosY() + pShooter->GetHeight() * .25f);
	//////////////////////////////
	// Bug #6 Fix
	//
	// Sam Mathis
	//////////////////////////////
	//float SpeedX =  ( (pTarget->GetPosX() + pTarget->GetWidth()  * .5f) - newWind->GetPosX() ) / 100;
	//float SpeedY =  ( (pTarget->GetPosY() + pTarget->GetHeight() * .5f) - newWind->GetPosY() ) / 100;
	//newWind->SetVelX( SpeedX * 250 );
	//newWind->SetVelY( SpeedY * 250 );

	float xComp = (pTarget->GetPosX() + pTarget->GetWidth()  * .5f) - newWind->GetPosX();
	float yComp = (pTarget->GetPosY() + pTarget->GetHeight() * .5f) - newWind->GetPosY();

	newWind->SetVelX( xComp / sqrt(( xComp * xComp + yComp * yComp )) * 275);
	newWind->SetVelY( yComp / sqrt(( xComp * xComp + yComp * yComp )) * 275);



	///////////////////////////
	// End Bug Fix
	//
	//////////////////////////
	newWind->SetDamage(CGameplayState::GetInstance()->GetDifficulty() + 1);
	newWind->SetLifespan(5.0f);
	newWind->SetActive(true);
	newWind->SetTier(1);

	newWind->SetWidth(32);
	newWind->SetHeight(16);

	Corona_ObjectManager::GetInstance()->AddObject(newWind);

	CEmitter* hahaiworknow = CParticleManager::GetInstance()->LoadEmitter("resource/data/windSpell.lapipt",0,0);
	newWind->SetEmitter(hahaiworknow);
	CParticleManager::GetInstance()->AddEmitter(newWind->GetEmitter());

	newWind->Release();
}

void CSpellFactory::CreateEnemyIce(CCharacter * pShooter, CBase * pTarget)
{
	CIce* newIce = new CIce();
	newIce->SetElement(OBJ_ICE);
	newIce->ShotBy(false);

	newIce->SetPosX(pShooter->GetPosX() + pShooter->GetWidth()  * .5f);
	newIce->SetPosY(pShooter->GetPosY() + pShooter->GetHeight() * .25f);
	//////////////////////////////
	// Bug #6 Fix
	//
	// Sam Mathis
	//////////////////////////////
	//float SpeedX =  ( (pTarget->GetPosX() + pTarget->GetWidth()  * .5f) - newIce->GetPosX() ) / 100;
	//float SpeedY =  ( (pTarget->GetPosY() + pTarget->GetHeight() * .5f) - newIce->GetPosY() ) / 100;
	//newIce->SetVelX( SpeedX * 250 );
	//newIce->SetVelY( SpeedY * 250 );

	float xComp = (pTarget->GetPosX() + pTarget->GetWidth()  * .5f) - newIce->GetPosX();
	float yComp = (pTarget->GetPosY() + pTarget->GetHeight() * .5f) - newIce->GetPosY();

	newIce->SetVelX( xComp / sqrt(( xComp * xComp + yComp * yComp )) * 200);
	newIce->SetVelY( yComp / sqrt(( xComp * xComp + yComp * yComp )) * 200);



	///////////////////////////
	// End Bug Fix
	//
	//////////////////////////

	newIce->SetDamage(10 * CGameplayState::GetInstance()->GetDifficulty());
	newIce->SetLifespan(5.0f);
	newIce->SetActive(true);
	newIce->SetTier(1);

	newIce->SetWidth(32);
	newIce->SetHeight(16);

	Corona_ObjectManager::GetInstance()->AddObject(newIce);

	CEmitter* hahaiworknow = CParticleManager::GetInstance()->LoadEmitter("resource/data/iceSpell.lapipt",0,0);
	newIce->SetEmitter(hahaiworknow);
	CParticleManager::GetInstance()->AddEmitter(newIce->GetEmitter());

	newIce->Release();
}
