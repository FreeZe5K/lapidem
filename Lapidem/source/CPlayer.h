#pragma once
#include "CCharacter.h"
#include "CLDevice.h"

class CPlayer : public CCharacter, public CLDevice
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
	int m_nScore;
	int m_nTierThree;
	bool m_bShielded;
	float m_fShieldTimer;

	void SetPlayerID( int _i )  { PlayerID = _i;      }
	void ToggleReticle();

public:
	CPlayer( );
	~CPlayer( );

	void Jump( );
	void Update( float );
	void Attack( int );
	void HandleCollision( CBase* );
	void HandleEvent(CEvent * pEvent);
	void SetShielded(bool bShielded) {m_bShielded = bShielded;}
	int TakeDamage( int nDamage )
	{
		if(!m_bShielded)
		{
			m_nHealth -= nDamage;
		}
		return 0;
	}


	int GetPlayerCount( )    { return PlayerCount;    }
	int GetPlayerID( )       { return PlayerID;       }
	int GetScore( )			 { return m_nScore;		  }
	int GetWindEnergy( )     { return m_nWindEnergy;  }
	int GetFireEnergy( )     { return m_nFireEnergy;  }
	int GetEarthEnergy( )    { return m_nEarthEnergy; }
	int GetIceEnergy( )      { return m_nWaterEnergy; }
	int GetT3Count( )	     { return m_nTierThree;   }
	CBase* GetReticle( )	 { return m_pReticle;	  }
};