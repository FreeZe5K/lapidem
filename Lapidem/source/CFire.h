//////////////////////////////////////////////////////////////////////////
//  File Name   :   "CFire.h"
//
//  Author Name :   Jonathan Caro
//
//  Purpose     :	Handles the fire objects created by the player and
//					enemies. Handles all 3 tiers of spells.
//////////////////////////////////////////////////////////////////////////
#ifndef CFIRE_H
#define CFIRE_H

#include "CSpell.h"
class CEmitter;

class CFire : public CSpell
{
	int m_nDOT;		//Determines the Damage Over Time to apply to the enemy or terrain
	CEmitter* m_pEmitter;

public:

	CFire();
	~CFire();

	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "Accessors"
	//
	//  Purpose   :   Get Stuff
	//////////////////////////////////////////////////////////////////////////
	int GetDOT() {return m_nDOT;}
	//////////////////////////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "Mutators"
	//
	//  Purpose   :   Set Stuff
	//////////////////////////////////////////////////////////////////////////
	void SetDOT(int nDOT) { m_nDOT = nDOT;}
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