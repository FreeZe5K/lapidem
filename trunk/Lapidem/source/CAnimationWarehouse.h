#pragma once

#include <windows.h>
#include <vector>
using std::vector;

class CAnimation;

class CAnimationWarehouse
{
private:
	// pointer to the instance of the class... 
	// only 1 should exist at a time
	static CAnimationWarehouse* AnimationWarehouse; 

	// vector... of a vector... of animations. 
	// Animation set storing a bunch of Animations
	vector<vector<CAnimation*>> m_vAnimationList;
	
	CAnimationWarehouse( )                                        { /* DO NOTHING */ }
	~CAnimationWarehouse( )                                       { /* DO NOTHING */ }
	CAnimationWarehouse( CAnimationWarehouse& )                   { /* DO NOTHING */ }
	CAnimationWarehouse& operator=( const CAnimationWarehouse& )  { /* DO NOTHING */ }

public:
	static CAnimationWarehouse* GetInstance( )
	{
		if(AnimationWarehouse == NULL)
			AnimationWarehouse = new CAnimationWarehouse;
		return AnimationWarehouse;
	}
	
	void DeleteInstance( );
	void LoadAnimationSet( char*, DWORD, char* );
	CAnimation* GetAnimation( int, int );
	void Init( );
};