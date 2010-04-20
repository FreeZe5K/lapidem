//////////////////////////////////////////////////////////////////////////
//  File Name   :   "CBase.h"
//
//  Author Name :   Jonathan Caro
//
//  Purpose     :   Provides the base functionality of all other classes 
//////////////////////////////////////////////////////////////////////////
#ifndef CBASE_H
#define CBASE_H
#include "IBaseInterface.h"
#include <windows.h>

enum Types{OBJ_BASE, OBJ_PLAYER, OBJ_FIRE, OBJ_EARTH, OBJ_WIND, OBJ_ICE};
class CBase : public IBaseInterface
{
	int m_nImageID;
	int m_nSound;
	float m_fPositionX;
	float m_fPositionY;
	float m_fVelocityX;
	float m_fVelocityY;
	int m_nWidth;
	int m_nHeight;
	int m_nRefCount;
	
protected:
	int m_nType;
public:

	CBase();
	~CBase();

	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "Accessors"
	//
	//  Purpose   :   Get Stuff
	//////////////////////////////////////////////////////////////////////////
	int GetImage(void) {return m_nImageID;}
	int GetSound(void) {return m_nSound;}
	int GetWidth(void) {return m_nWidth;}
	int GetHeight(void) {return m_nHeight;}
	float GetPosX(void) {return m_fPositionX;}
	float GetPosY(void) {return m_fPositionY;}
	float GetVelX(void) {return m_fVelocityX;}
	float GetVelY(void) {return m_fVelocityY;}
	int GetType(void) {return m_nType;}
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "Mutators"
	//
	//  Purpose   :   Set Stuff
	//////////////////////////////////////////////////////////////////////////
	void SetImage(int nImage) { m_nImageID = nImage;}
	void SetSound(int nSound) { m_nSound = nSound;}
	void SetWidth(int nWidth) { m_nWidth = nWidth;}
	void SetHeight(int nHeight) { m_nHeight = nHeight;}
	void SetPosX(float fPosX) {m_fPositionX = fPosX;}
	void SetPosY(float fPosY) {m_fPositionY = fPosY;}
	void SetVelX(float fVelX) {m_fVelocityX = fVelX;}
	void SetVelY(float fVelY) {m_fVelocityY = fVelY;}
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "GetCollisionRect"
	//
	//  Purpose   :   Returns the collision rect of the object.
	//////////////////////////////////////////////////////////////////////////
	RECT GetCollisionRect();
	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "CheckCollision"
	//
	//  Purpose   :   Checks collision against passed in CBase. Handling of 
	//				  collision can also be handled here.
	//////////////////////////////////////////////////////////////////////////
	bool CheckCollision (CBase* pbase);


	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "Update"
	//
	//  Purpose   :   This method is in charge of updating the object based on
	//				  its position and velocity. If Input is needed, it should
	//				  be added before update code.
	//////////////////////////////////////////////////////////////////////////
	virtual void Update( float  fElapsedTime);

	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "Render"
	//
	//  Purpose   :   This method is responsible for drawing the object to 
	//				  the screen.
	//////////////////////////////////////////////////////////////////////////
	virtual void Render( ) ;

	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "AddRef"
	//
	//  Purpose   :   This method is used to keep track of how many instances
	//				  of this object are being shared elsewhere.
	//////////////////////////////////////////////////////////////////////////
	void AddRef( )  
	{
		m_nRefCount++;
	}

	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "Release"
	//
	//  Purpose   :   This method is tasked with removing a counter from
	//				  m_nRefCount. Once m_nRefCount has reached 0, the object 
	//				  is cleared to be deleted/removed.
	//////////////////////////////////////////////////////////////////////////
	void Release( )
	{
		m_nRefCount--;
		if(m_nRefCount ==0)
		{
			delete this;
		}
	}
};


#endif