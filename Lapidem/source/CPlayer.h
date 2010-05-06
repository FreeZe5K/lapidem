#pragma once
#include "CCharacter.h"
#include "CGameOver.h"

class CGameOver;

class CPlayer : public CCharacter
{
	bool m_bIsJumping;
	float m_fJumpTimer;
	float m_fFireTimer;

	CBase* m_pReticle;

	static int PlayerCount;
	int PlayerID;
	int m_nFireEnergy;
	int m_nWaterEnergy; // I know it's Ice, stfu;
	int m_nWindEnergy;
	int m_nEarthEnergy;
	int m_nTierThree;

	void SetPlayerID( int _i )  { PlayerID = _i;      }
	void ToggleReticle();

public:
	CPlayer( );
	~CPlayer( )                 { --PlayerCount; if(m_pReticle)ToggleReticle();    }

	void Jump( );
	void Update( float );
	void Attack( int );
	void HandleCollision( CBase* );

	int GetPlayerCount( )    { return PlayerCount;    }
	int GetPlayerID( )       { return PlayerID;       }
	CBase* GetReticle( )	 { return m_pReticle;	  }
};