#ifndef AISTATEWIND_H
#define AISTATEWIND_H

#include "IAIState.h"
#include "CFlock.h"
class CEnemy;
class CCharacter;
class CLevel;
class CBase;

class AIStateWind : public IAIState
{
	bool m_bAttacking;
	float m_fInfluence;
	float m_fShudderTimer;	//to tell the enemy to adjust slightly from the flock to keep all enemies from having the same velocity over time
							//this value is set to a random number at the init of the AI state
	CFlock* m_pFlock;
	CPlayer* m_pTarget;
public:

	AIStateWind();
	~AIStateWind();
	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "Update"
	//
	//	Input	  :	  Takes in a float for time, as well as CEnemy*
	//
	//  Purpose   :   Movement and deciding when and who to attack
	//////////////////////////////////////////////////////////////////////////
	int Update( float, CEnemy* );



	void SetFlock(CFlock* pFlock) { m_pFlock = pFlock;}
	CFlock* GetFlock(void) {return m_pFlock;}


	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "Attack"
	//
	//	Input	  :	  Takes in the character to shoot at as well as the enemy
	//				  shooting. 
	//
	//  Purpose   :   Use attack by calling spell factory
	//////////////////////////////////////////////////////////////////////////
	void Attack( CCharacter*, CCharacter*);

	int AttackUpdate(CPlayer* pTarget, CEnemy* pEnemy, float fElapsedTime);

	bool CheckPassable(CLevel* pLevel, CBase* pObject, float fElapsedTime);

	void HandleCollision(CEnemy* pBird);	//CEnemy handle collision calls this when collided with another wind enemy to adjust their movement

};

#endif
