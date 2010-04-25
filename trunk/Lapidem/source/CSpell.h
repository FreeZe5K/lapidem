//////////////////////////////////////////////////////////////////////////
//  File Name   :   "CSpell.h"
//
//  Author Name :   Jonathan Caro
//
//  Purpose     :   Provides the functionality for the spells to inherit 
//////////////////////////////////////////////////////////////////////////
#ifndef CSPELL_H
#define CSPELL_H
#include "CBase.h"



///////////////////////////////////////////
//TODO: Once Emitter is finished uncomment
///////////////////////////////////////////

//////////////////////////
// #include "CEmitter.h"
//////////////////////////

class CSpell : public CBase//, public CEmitter
{
	float m_fLifespan;	// How long the spell will live
	bool m_bPlayer;		// Determines whether the player shot the bullet (True == Player Shot)
	int m_nTier;		// Determines the tier of the spell as well as the update and render for the spell
	int m_nDamage;		// How much damage the spell does on collision with an object
	int m_nDirection;	// Enumed(sp?) to represent a direction the spell is  beign shot at
	int m_nElement;		// Enum for which element the spell is... Fire Earth Wind Ice
public:

	CSpell();	// constructor
	~CSpell();	// destructor

	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "Accessors"
	//
	//  Purpose   :   Get Stuff
	//////////////////////////////////////////////////////////////////////////
	float GetLifespan(void) {return m_fLifespan;}
	bool PlayerShot(void) {return m_bPlayer;}
	int GetTier(void) {return m_nTier;}
	int GetDamage(void) {return m_nDamage;}
	int GetDirection(void) {return m_nDirection;}
	int GetElement(void) {return m_nElement;}
	//////////////////////////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "Mutators"
	//
	//  Purpose   :   Set Stuff
	//////////////////////////////////////////////////////////////////////////
	void SetLifespan(float fLifespan) {m_fLifespan = fLifespan;}
	void ShotBy(bool bPlayer) {m_bPlayer = bPlayer;}
	void SetTier(int nTier) {m_nTier = nTier;}
	void SetDamage(int nDamage) {m_nDamage = nDamage;}
	void SetDirection(int nDirection) {m_nDirection = nDirection;}
	void SetElement(int nElement) {m_nElement = nElement;}
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "CheckCollision"
	//
	//  Purpose   :   Checks collision against passed in CBase. Handle
	//				  Collision is called here in event of collision
	//////////////////////////////////////////////////////////////////////////
	bool CheckCollision(CBase* pbase);

	
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
	virtual void Update(float fElapsedTime);

	virtual void UpdateTier1(float fElapsedTime);
	virtual void UpdateTier2(float fElapsedTime);
	virtual void UpdateTier3(float fElapsedTime);

	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "Render"
	//
	//  Purpose   :   This method is responsible for drawing the object to 
	//				  the screen. This function calls the corresponding
	//				  Render function for each tier.
	//////////////////////////////////////////////////////////////////////////
	virtual void Render(void);

	virtual void RenderTier1(void);
	virtual void RenderTier2(void);
	virtual void RenderTier3(void);
	
	


};


#endif