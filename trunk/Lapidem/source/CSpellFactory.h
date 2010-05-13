//////////////////////////////////////////////////////////////////////////
//  File Name   :   "CSpellFactory.h"
//
//  Author Name :   Jonathan Caro
//
//  Purpose     :   Used to create all spells. Also handles the experience
//					gains and levels of each element. After Creation, the
//					spell is put into the ObjectFactory as well as 
//					returned.
//////////////////////////////////////////////////////////////////////////
#pragma once

#include "CParticleManager.h"

class CCharacter;
class CBase;
class CSpell;
class CIce;
class CFire;
class CEarth;
class CWind;
class CEmitterFactory;

class CSpellFactory
{
private:
	static  CSpellFactory* m_pSF;    // Instance to itself
	int     m_nIceXP;                // How much XP the ice abilities have accumulated. Used to level up the ice abilities.
	int     m_nIceLVL;               // Current level of ice abilities. Used to determine damage increase as well as slow bonus.

	int     m_nWindXP;               // How much XP the wind abilities have accumulated. Used to level up the wind abilities
	int     m_nWindLVL;              // Current level of wind abilities. Used to determine damage increase as well as pushback bonus.

	int     m_nFireXP;               // How much XP the fire abilities have accumulated. Used to level up the fire abilities.
	int     m_nFireLVL;              // Current level of fire abilities. Used to determine damage increase as well as DOT.

	int     m_nEarthXP;              // How much XP the earth abilities have accumulated. Used to level up the earth abilities
	int     m_nEarthLVL;             // Current level of earth abilities. Used to determine damage increase as well as lifespan.

	CEmitterFactory* m_pEF;

public:
	CSpellFactory( );
	~CSpellFactory( );

	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "CreateIce"
	//
	//	Input	  :	  pShooter	-	The object that created the spell
	//				  nTier		-	The level of the spell to create	
	//
	//  Purpose   :   Create and put into object manager an Ice spell based on tier provided
	//////////////////////////////////////////////////////////////////////////
	void CreateIce( CCharacter* pShooter, int nTier );

	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "CreateFire"
	//
	//	Input	  :	  pShooter	-	The object that created the spell
	//				  nTier		-	The level of the spell to create	
	//
	//  Purpose   :   Create and put into object manager a Fire spell based on tier provided
	//////////////////////////////////////////////////////////////////////////
	void CreateFire( CCharacter* pShooter, int nTier );
	void CreateEnemyFire( CCharacter * pShooter, CBase* pTarget);

	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "CreateWind"
	//
	//	Input	  :	  pShooter	-	The object that created the spell
	//				  nTier		-	The level of the spell to create	
	//
	//  Purpose   :   Create and put into object manager a Wind spell based on tier provided
	//////////////////////////////////////////////////////////////////////////
	void CreateWind( CCharacter* pShooter, int nTier );

	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "CreateEarth"
	//
	//	Input	  :	  pShooter	-	The object that created the spell
	//				  nTier		-	The level of the spell to create	
	//
	//  Purpose   :   Create and put into object manager an Earth spell based on tier provided
	//////////////////////////////////////////////////////////////////////////
	void CreateEarth( CCharacter* pShooter, int nTier );

	void CreateGrenade(CSpell* pFire, CSpell* pEarth);
	void CreateGiantFireBall(CSpell* pFire, CSpell* pWind);
	void CreateSpear(CSpell* pIce, CSpell* pWind);
	void CreateIceCube(CSpell* pIce, CSpell* pEarth);

	//void CreateSparks(int Type, float fDirRotation, float fSparkLifeTime);

	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "GetInstance"	
	//
	//  Purpose   :   Return an instance to the Spell Factory to be used.
	//				  If none exists, the spell factory will be created.
	//////////////////////////////////////////////////////////////////////////
	static CSpellFactory* GetInstance( ) 
	{
		if( 0 == m_pSF )
			m_pSF = new CSpellFactory( );
		return m_pSF;
	}

	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "DeleteInstance"	
	//
	//  Purpose   :   Deletes the instance to the Spell Factory. To be used
	//				  with shutdown or restart of the game.
	//////////////////////////////////////////////////////////////////////////
	void DeleteInstance( )
	{
		if( m_pSF != 0 )
		{
			delete m_pSF;
			m_pSF = 0;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "Mutators"	
	//
	//  Purpose   :   Change stuff
	//////////////////////////////////////////////////////////////////////////
	void AddWindXP( int );
	void AddEarthXP( int );
	void AddFireXP( int );
	void AddIceXP( int );

	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "Accessors"	
	//
	//  Purpose   :   Get stuff
	//////////////////////////////////////////////////////////////////////////
	int GetFireLevel( )     { return m_nFireLVL;      }
	int GetIceLevel( )      { return m_nIceLVL;       }
	int GetEarthLevel( )    { return m_nEarthLVL;     }
	int GetWindLevel( )     { return m_nWindLVL;      }
};