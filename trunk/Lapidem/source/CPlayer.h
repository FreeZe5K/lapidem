#pragma once
#include "CCharacter.h"

class CPlayer : public CCharacter
{
	bool m_bIsJumping;
	float m_fJumpTimer;

	float m_fFireTimer;

	static int PlayerCount;
	int PlayerID;

	void SetPlayerID( int _i )  { PlayerID = _i;      }

public:
	CPlayer( );
	~CPlayer( )                 { --PlayerCount;      }

	void Jump( );
	void Update( float );
	void Attack( int );
	void HandleCollision( CBase* );

	int GetPlayerCount( )    { return PlayerCount;    }
	int GetPlayerID( )       { return PlayerID;       }
};