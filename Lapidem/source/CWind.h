//////////////////////////////////////////////////////////////////////////
//  File Name   :   "CWind.h"
//
//  Author Name :   Jonathan Caro
//
//  Purpose     :	Handles the wind objects created by the player and
//					enemies. Handles all 3 tiers of spells.
//////////////////////////////////////////////////////////////////////////
#pragma once

#include "CSpell.h"

class CWind : public CSpell
{
private:
	 // How much the unit being hit 
	// by the air spell gets pushed back
	float m_fPushBack;

public:
	CWind( );
	~CWind( );

	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "Accessors"
	//
	//  Purpose   :   Get Stuff
	//////////////////////////////////////////////////////////////////////////
	float GetPushBack( )                 { return m_fPushBack;       }
	
	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "Mutators"
	//
	//  Purpose   :   Set Stuff
	//////////////////////////////////////////////////////////////////////////
	void SetPushBack( float fPushBack )  { m_fPushBack = fPushBack;  }

	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "HandleCollision"
	//
	//  Purpose   :   The effect of the collision including object destruction
	//			      and damage dealing is done here.
	//////////////////////////////////////////////////////////////////////////
	void HandleCollision( CBase* );

	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "Update"
	//
	//  Purpose   :   This method is in charge of updating the object based on
	//				  its position and velocity. This function calls the 
	//				  the corresponding update for each tier.
	//////////////////////////////////////////////////////////////////////////
	void Update( float );
	void UpdateTier1( float );
	void UpdateTier2( float );
	void UpdateTier3( float );

	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "Render"
	//
	//  Purpose   :   This method is responsible for drawing the object to 
	//				  the screen. This function calls the corresponding
	//				  Render function for each tier.
	//////////////////////////////////////////////////////////////////////////
	void Render( );
	void RenderTier1( );
	void RenderTier2( );
	void RenderTier3( );
	
	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "CheckCollision"
	//
	//  Purpose   :   Checks collision against passed in CBase. Handle
	//				  Collision is called here in event of collision
	//////////////////////////////////////////////////////////////////////////
	bool CheckCollision( CBase* );
};