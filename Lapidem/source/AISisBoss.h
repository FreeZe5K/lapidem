#pragma once

#include "IAIState.h"
#include "CEnemy.h"

class CPlayer;
class CLevel;

class AIStateEarth;
class AIStateIce;
class AIStateFire;

#define ATTACK_TIME 5.0f
#define V_TIME 2.0f
#define ATTACK_RATE 2.0f
#define BURST_TIMER 15.0f

class AISisBoss : public IAIState
{
	int m_nInitX;
	int m_nInitY;

	bool m_bIsJumping;

	int m_nCurrentElement;	// for attack
	float m_fVulnerableTimer;
	CPlayer* m_pTarget;

	float m_fAttackCooldown;
	float m_fBurstCooldown;
	
	IAIState* m_pStates[4];

public:
	AISisBoss();
	~AISisBoss();

	void SetCurrElement(int Element) { m_nCurrentElement = Element; }
	int GetCurrElement() { return m_nCurrentElement; }

	int Update(float, CEnemy*);
	void HandleMovement(float, CEnemy*);
	void Attack(CCharacter*, CCharacter*);	

	bool CheckPassable(CLevel* pLevel, CBase* pObject, float fElapsedTime);

	void Jump() { m_bIsJumping = true; }
	void ResetJump() { m_bIsJumping = false; }

	bool IsLanded(CCharacter* pObject, float fElapsedTime);
	int GetJumpDirection(CCharacter* pObject, float fElapsedTime);
	
	//void CreateElementBurst(CCharacter* pObject);
};