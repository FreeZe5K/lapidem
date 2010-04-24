#pragma once

class CCharacter;
class CEnemy;

class IAIState
{


public:
	virtual bool Update(float, CEnemy*) = 0;
	virtual void Attack(CCharacter*) = 0;


};