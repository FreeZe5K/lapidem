#ifndef CEARTH_H
#define CEARTH_H

#include "CSpell.h"

class CEarth : public CSpell
{
	float m_fRotate;			//Used for the basic earth bolt. The bolt is "lobbed" and slowly rotated forwards it falls.	In Radians
	
	float m_fTimeTillRotate;	//When the basic earth bolt is created, it travels at a steady velocity, after the timer has expired
								//the bolt begins to rotate forward until it collides with another object, at that point the rotation
								// is set to 0 and the bolt should end up landing flat on a surface (unless it hits an enemy in which case it dissapears). 
	
	float m_fDisplay;		// How much the boulder should be rotated when displaying

	bool collided;

public:
	CEarth();
	~CEarth();
	
	
	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "Mutators"
	//
	//  Purpose   :   Set Stuff
	//////////////////////////////////////////////////////////////////////////
	void SetRotate(float fRotation) { m_fRotate = fRotation;}
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "HandleCollision"
	//
	//  Purpose   :   The effect of the collision including object destruction
	//			      and damage dealing is done here.
	//////////////////////////////////////////////////////////////////////////
	void HandleCollision(CBase* pObject);


	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "Update"
	//
	//  Purpose   :   This method is in charge of updating the object based on
	//				  its position and velocity. This function calls the 
	//				  the corresponding update for each tier.
	//////////////////////////////////////////////////////////////////////////
	void Update(float fElapsedTime);
	void UpdateTier1(float fElapsedTime);
	void UpdateTier2(float fElapsedTime);
	void UpdateTier3(float fElapsedTime);


	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "Render"
	//
	//  Purpose   :   This method is responsible for drawing the object to 
	//				  the screen. This function calls the corresponding
	//				  Render function for each tier.
	//////////////////////////////////////////////////////////////////////////
	void Render(void);
	void RenderTier1(void);
	void RenderTier2(void);
	void RenderTier3(void);
	
	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "CheckCollision"
	//
	//  Purpose   :   Checks collision against passed in CBase. Handle
	//				  Collision is called here in event of collision
	//////////////////////////////////////////////////////////////////////////
	bool CheckCollision(CBase* pbase);
};
#endif

/*

Trajectory
   []
  v	  v
 <>   <>
O     []
*/