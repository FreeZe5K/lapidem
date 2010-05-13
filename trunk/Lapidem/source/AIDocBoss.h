#pragma once
#include "IAIState.h"

class CCharacter;
class CEnemy;


class AIDocBoss : public IAIState
{
	bool m_bIsShielded;
	float m_fShieldTimer;
	float m_fSpawnEnemyTimer;
	int m_nMaxHealth;


public:
	AIDocBoss();
	int Update(float fElapsedTime, CEnemy* thaBoss);
	void Attack(CCharacter*, CCharacter*);
	void SpawnEnemy();



};