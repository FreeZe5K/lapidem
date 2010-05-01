#pragma once
#include "CCharacter.h"

class CPlayer : public CCharacter
{
	bool m_bIsJumping;
	float m_fJumpTimer;

	float m_fFireTimer;

	static int PlayerCount;
	int PlayerID;


	void SetPlayerID(int pID) {PlayerID = pID;}

public:
	CPlayer();
	~CPlayer() {--PlayerCount;}
	void Update(float fElapsedTime);
	void Attack(int nTier);
	void Jump();
	void HandleCollision(CBase *);

	int GetPlayerCount() { return PlayerCount;}
	int GetPlayerID()	 { return PlayerID;}

};