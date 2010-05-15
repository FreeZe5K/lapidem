//////////////////////////////////////////////////////////////////////////
//  File Name   :   "CEarth.h"
//
//  Author Name :   Jonathan Caro
//
//  Purpose     :	Handles the earth objects created by the player and
//					enemies. Handles all 3 tiers of spells.
//////////////////////////////////////////////////////////////////////////
#pragma once
#include "CSpell.h"
#include "Lapidem_Math.h"
#include "CLDevice.h"

class CEarth : public CSpell, public CLDevice
{
private:
	//Used for the basic earth bolt. 
	// The bolt is "lobbed" and slowly 
	// rotated forwards it falls. In Radians.
	float  m_fRotate;
	//When the basic earth bolt is created, it travels 
	// at a steady velocity, after the timer has expired
	//the bolt begins to rotate forward until it collides 
	// with another object, at that point the rotation
	// is set to 0 and the bolt should end up landing flat 
	// on a surface (unless it hits an enemy in which case it dissapears). 
	float  m_fTimeTillRotate;
	
	float m_fDisplay;
	float  m_fRiseAmount;
	float m_fXScale;
	float m_fYScale;
	bool   collided;
	bool  m_bSunk;
	bool  m_bIsIce;
	float m_fCrumbleTimer;


public:
	CEarth( );
	~CEarth( );
	
	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "Mutators"
	//
	//  Purpose   :   Set Stuff
	//////////////////////////////////////////////////////////////////////////
	void SetRotate( float fRotation ) { m_fRotate = fRotation; }
	void SetDisplay(float fDisplay) { m_fDisplay = fDisplay; }
	void SetXScale(float fScale) { m_fXScale = fScale;}
	void SetYScale(float fScale) { m_fYScale = fScale;}



	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "HandleCollision"
	//
	//  Purpose   :   The effect of the collision including object destruction
	//			      and damage dealing is done here.
	//////////////////////////////////////////////////////////////////////////
	void HandleCollision(float, CBase* );
	void HandleEvent(CEvent *pEvent);

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