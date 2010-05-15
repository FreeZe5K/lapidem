//////////////////////////////////////////////////////////////////////////
//  File Name   :   "CIce.h"
//
//  Author Name :   Jonathan Caro
//
//  Purpose     :	Handles the ice objects created by the player and
//					enemies. Handles all 3 tiers of spells.
//////////////////////////////////////////////////////////////////////////
#pragma once
#include "CSpell.h"

class CIce : public CSpell
{
private:
	// After being hit by an ice spell, 
	// the units velocity is reduced by this amount.
	float m_fSlowRate; 

public:
	CIce( );
	~CIce( );

	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "Accessors"
	//
	//  Purpose   :   Get Stuff
	//////////////////////////////////////////////////////////////////////////
	float GetSlow( )                { return m_fSlowRate;       }

	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "Mutators"
	//
	//  Purpose   :   Set Stuff
	//////////////////////////////////////////////////////////////////////////
	void SetSlow( float fSlow )     { m_fSlowRate = fSlow;      }

	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "HandleCollision"
	//
	//  Purpose   :   The effect of the collision including object destruction
	//			      and damage dealing is done here.
	//////////////////////////////////////////////////////////////////////////
	void HandleCollision(float, CBase* );

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

};