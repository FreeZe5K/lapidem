//////////////////////////////////////////////////////////////////////////
//  File Name   :   "CCharacter.h"
//
//  Author Name :   Sam Mathis
//
//  Purpose     :	Control the movement and attacks of the Earth enemies
//////////////////////////////////////////////////////////////////////////
#pragma once
#include "CBase.h"
#include "CLDevice.h"
#include "CSpellFactory.h"

class CAnimation;

enum DIRECTION{ LEFT, RIGHT, UP, DOWN, LEFT_UP, LEFT_DOWN, RIGHT_UP, RIGHT_DOWN, NO_DIRECTION };

class CCharacter : public CBase
{
protected:
	CAnimation*     animation;
	bool            IsRotated;
	int             m_nHealth;
	int             currAnimation;
	CSpellFactory*  m_pSpells;
	EleType         m_SpellType;
	DIRECTION       currDirec;

public:
	CCharacter( );
	void Render( ); 
	void SetEleType( EleType );
	void Update( float );
	void SetAnimation( int, int, int frame = -1 );

	int GetHealth( )                    { return m_nHealth;     }
	DIRECTION GetDirection( )           { return currDirec;     }
	EleType GetEleType( )               { return m_SpellType;   }

	virtual int TakeDamage( int nDamage );
	void SetHealth( int nHealth )				{ m_nHealth = nHealth;  }
	void SetDirection( DIRECTION newDirec)		{ currDirec = newDirec; }
};