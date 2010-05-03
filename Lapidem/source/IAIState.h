#pragma once

class CCharacter;
class CEnemy;

class IAIState
{
public:
	virtual int Update( float, CEnemy* )              = 0;
	virtual void Attack( CCharacter*, CCharacter* )   = 0;
};