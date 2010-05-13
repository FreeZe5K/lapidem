#ifndef CFLOCK_H
#define CFLOCK_H

#include "CBase.h"
#include <vector>
using namespace std;
class CEnemy;
class CPlayer;

class CFlock : public CBase
{
	vector<CEnemy*> m_vFlock;
	CEnemy* m_pLeader;
	CPlayer* m_pAttackTarget;
	bool m_bAttacking;
	float m_fInfluence;		// radius 
	float m_fShockTimer;	// timer for confusion after leader dies before reforming
	int m_nFlockSize;
	
	//solo bird... have a timer for randomly adjusting velocity and direction based on time to create smooth flight path
	//solo bird will have timer to stop adjusting as well
	void ChangeLeader(void);			// new leader... adjusting the radius position and who calls update on the flock
	
public:


	//

	CFlock();
	~CFlock();	
	
	CEnemy* GetLeader(void) {return m_pLeader;}
	
	void AddMember(CEnemy* addme);		// add to vector as well as adjust  influence... first member added is now the leader
	void RemoveMember(CEnemy* removeme);	// remove from vector as well as adjust influence... if flock size is 0 delete self
	void Update(float fElapsedTime);	// set position = to leader's position. 
	
	bool GetShocked(void);			// to see if the leader died recently

	float CalculateDistance(float OrigPosX, float OrigPosY, float NewPosX, float NewPosY);
	
	float GetAverageXVel(float ,float, float);
	float GetAverageYVel(float, float, float);

	bool InRadius(CEnemy* pBird);	// checks to see if the bird is within the radius of the flock

	void HandleCollision(CBase* pObject);	// check to see if another flock is within range to merge the flocks, if a flock is attacking, 
											// that flock takes priority, otherwise it is based off size of the flock
											// leader will check whether the flock is colliding with another flock.
	
	void MergeFlocks(CFlock* pMergeFlock, CFlock* pDestroyedFlock);


	//INLINES
	int GetFlockSize(void) {return m_vFlock.size();} // to see if the flock leader is the only bird in the flock
	
	bool IsAttacking(void) {return m_bAttacking;}	// to see if they should take normal motion or attacking formation used in aistate

	void SetAttacking(bool bAttacking) {m_bAttacking = bAttacking;} 

	float GetInfluence(void) {return m_fInfluence;}

	void SetTarget(CPlayer* pTarget) {m_pAttackTarget = pTarget;}

	CPlayer* GetTarget(void) {return m_pAttackTarget;}
	
	vector<CEnemy*> GetFlock(void) {return m_vFlock;}
	



};


#endif