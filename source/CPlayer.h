#pragma once
#include "CCharacter.h"
#include "CLDevice.h"

class CCamera;

class CPlayer : public CCharacter, public CLDevice
{
	bool m_bIsJumping;
	float m_fFireTimer;

	CBase* m_pReticle;

	static int PlayerCount;
	int PlayerID;
	int m_nFireEnergy;
	int m_nWaterEnergy; // I know it's Ice, stfu;
	int m_nWindEnergy;
	int m_nEarthEnergy;
	int m_nScore;
	int m_nAttack;
	int m_nTierThree;
	bool m_bShielded;
	bool m_bIsTouching;
	float m_fDrownTimer;//Seriously?
	float m_fShieldTimer;
	float RetPosX;
	float RetPosY;

	void SetPlayerID( int _i )  { PlayerID = _i;      }
	void ToggleReticle();

	bool Tossed;

	bool m_bFainted;

public:
	CPlayer( );
	~CPlayer( );

	void Jump( );
	void Update( float );
	void Attack( int );
	void Render(void);
	void HandleCollision(float, CBase* );
	void HandleEvent(CEvent * pEvent);
	void SetShielded(bool bShielded) {m_bShielded = bShielded;}

	_inline int TakeDamage( int nDamage )
	{
		if( !m_bShielded )
			m_nHealth = m_nHealth - nDamage;
		return 0;
	}

	_inline void ResetJump( )
	{ m_bIsJumping = false; }

	RECT GetCollisionRect(float fElapsedTime);

	void SetFainted( bool bFainted ) { m_bFainted = bFainted; }
	bool GetFainted() { return m_bFainted; }

	int GetPlayerCount( )    { return PlayerCount;    }
	int GetAttackType()		 { return m_nAttack;	  }
	CAnimation* GetAnimation(){return animation;	  }
	int GetAnimationType()	 { return currAnimation;  }
	char* GetTrigger();
	int GetPlayerID( )       { return PlayerID;       }
	int GetScore( )			 { return m_nScore;		  }
	int GetWindEnergy( )     { return m_nWindEnergy;  }
	int GetFireEnergy( )     { return m_nFireEnergy;  }
	int GetEarthEnergy( )    { return m_nEarthEnergy; }
	int GetIceEnergy( )      { return m_nWaterEnergy; }
	int GetT3Count( )	     { return m_nTierThree;   }
	bool GetTossed( )		 { return Tossed;		  }
	CBase* GetReticle( )	 { return m_pReticle;	  }

	void setWindEnergy( int _i )  { m_nWindEnergy   = _i; }
	void setFireEnergy( int _i )  { m_nFireEnergy   = _i; }
	void setEarthEnergy( int _i ) { m_nEarthEnergy  = _i; }
	void setIceEnergy( int _i )   { m_nWaterEnergy  = _i; }
	void SetAttack( int _i);
	///Animation Protection
	///Energy Level consideration;

};