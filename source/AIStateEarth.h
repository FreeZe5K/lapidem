//////////////////////////////////////////////////////////////////////////
//  File Name   :   "AIStateEarth.h"
//
//  Author Name :   Jonathan Caro
//
//  Purpose     :	Control the movement and attacks of the Earth enemies
//////////////////////////////////////////////////////////////////////////
#pragma once
#include "IAIState.h"

class CLevel;
class CBase;

class AIStateEarth : public IAIState
{
	int InitX;	// gives the initial position of the earth enemy to determine wander range
	int InitY;	// give the initial position of the earth enemy

public:
	~AIStateEarth() { }
	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "Update"
	//
	//	Input	  :	  Takes in a float for time, as well as CEnemy*
	//
	//  Purpose   :   Movement and deciding when and who to attack
	//////////////////////////////////////////////////////////////////////////
	int Update( float, CEnemy* );

	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "Attack"
	//
	//	Input	  :	  Takes in the character to shoot at as well as the enemy
	//				  shooting. 
	//
	//  Purpose   :   Use attack by calling spell factory
	//////////////////////////////////////////////////////////////////////////
	void Attack( CCharacter*, CCharacter*);
	void SetInitPos( int x, int y ) { InitX = x, InitY = y; }

	bool CheckPassable(CLevel* pLevel, CBase* pObject, float fElapsedTime);
};