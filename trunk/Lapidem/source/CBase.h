//////////////////////////////////////////////////////////////////////////
//  File Name   :   "CBase.h"
//
//  Author Name :   Jonathan Caro
//
//  Purpose     :   Provides the base functionality of all other classes.
//////////////////////////////////////////////////////////////////////////
#pragma once
#include "IBaseInterface.h"
#include <windows.h>

enum Types{ OBJ_BASE, OBJ_PLAYER, OBJ_FIRE, OBJ_EARTH, OBJ_WIND, OBJ_ICE, OBJ_TERRA }; //All Tiles need to have "at least" the OBJ_TERRA type.

class CBase : public IBaseInterface
{
private:
	int    m_nImageID;
	int    m_nSound;
	int    m_nWidth;
	int    m_nHeight;
	int    m_nRefCount;

	float  m_fPositionX;
	float  m_fPositionY;
	float  m_fVelocityX;
	float  m_fVelocityY;

	bool   m_bIsActive;
	
protected:
	int m_nType;

public:
	CBase( );
	~CBase( );

	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "Accessors"
	//
	//  Purpose   :   Get Stuff
	//////////////////////////////////////////////////////////////////////////
	int   GetImage( )     {    return m_nImageID;        }
	int   GetSound( )     {    return m_nSound;          }
	int   GetWidth( )     {    return m_nWidth;          }
	int   GetHeight( )    {    return m_nHeight;         }
	int   GetType( )      {    return m_nType;           }

	float GetPosX( )      {    return m_fPositionX;      }
	float GetPosY( )      {    return m_fPositionY;      }
	float GetVelX( )      {    return m_fVelocityX;      }
	float GetVelY( )      {    return m_fVelocityY;      }

	bool IsActive( )	  {	   return m_bIsActive;		 }

	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "Mutators"
	//
	//  Purpose   :   Set Stuff
	//////////////////////////////////////////////////////////////////////////
	void SetImage( int nImage )     {    m_nImageID     = nImage;   }
	void SetSound( int nSound )     {    m_nSound       = nSound;   }
	void SetWidth( int nWidth )     {    m_nWidth       = nWidth;   }
	void SetHeight( int nHeight )   {    m_nHeight      = nHeight;  }
	void SetType( int nType )		{	 m_nType		= nType;	}

	void SetPosX( float fPosX )     {    m_fPositionX   = fPosX;    }
	void SetPosY( float fPosY )     {    m_fPositionY   = fPosY;    }
	void SetVelX( float fVelX )     {    m_fVelocityX   = fVelX;    }
	void SetVelY( float fVelY )     {    m_fVelocityY   = fVelY;    }
	void SetActive( bool bIsActive) {    m_bIsActive	= bIsActive;}

	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "GetCollisionRect"
	//
	//  Purpose   :   Returns the collision rect of the object.
	//////////////////////////////////////////////////////////////////////////
	RECT GetCollisionRect( );

	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "CheckCollision"
	//
	//  Purpose   :   Checks collision against passed in CBase. Handling of 
	//				  collision can also be handled here.
	//////////////////////////////////////////////////////////////////////////
	bool CheckCollision( CBase* );
	
	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "Update"
	//
	//  Purpose   :   This method is in charge of updating the object based on
	//				  its position and velocity. If Input is needed, it should
	//				  be added before update code.
	//////////////////////////////////////////////////////////////////////////
	virtual void Update( float );

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

		if( 0 == m_nRefCount )
			delete this;
	}
};