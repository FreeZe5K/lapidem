#include "CCamera.h"
#include "CBase.h"
#include "CPlayer.h"

CCamera * CCamera::theCamera = 0;

void CCamera::InitCamera( float fPosX, float fPosY, float fWidth, float fHeight, CBase* ObjectToFollow, CBase* SecondObject )
{
	if( !theCamera )
	{
		theCamera = new CCamera( );

		theCamera->SetCameraXOffset( fPosX );
		theCamera->SetCameraYOffset( fPosY );
		theCamera->SetCameraWidth( fWidth );
		theCamera->SetCameraHeight( fHeight );

		theCamera->thePlayer = ObjectToFollow;
		theCamera->thePlayer2 = SecondObject;

		theCamera->SetVelocityX( 0.0f );
		theCamera->SetVelocityY( 0.0f );
	}
}

void CCamera::ShutDownCamera( )
{
	if( theCamera )
	{
		delete theCamera;
		theCamera = NULL;
	}
}

void CCamera::Update( float fElapsedTime )
{
	int width( int( theCamera->GetWidth( )-theCamera->GetXOffset( ) ) );
	int height( int( theCamera->GetHeight( )-theCamera->GetYOffset( ) ) );

	RECT rCamera;
	rCamera.left    = LONG( GetXOffset() );
	rCamera.right   = LONG( rCamera.left + GetWidth() );
	rCamera.top     = LONG( GetYOffset() );
	rCamera.bottom  = LONG( rCamera.top + GetHeight() );

	RECT rPlayer;
	if( !((CPlayer*)thePlayer)->GetFainted() )
	 rPlayer    = thePlayer->GetCollisionRect( fElapsedTime );
	else
	 rPlayer    = thePlayer2->GetCollisionRect( fElapsedTime );



	int CenterOfScreenX( int( theCamera->GetXOffset( ) + width / 2 ) );
	int CenterOfScreenY( int( theCamera->GetYOffset( ) + height / 2 ) );

	RECT rHook, r;
	int x          = thePlayer->GetWidth( );
	rHook.left     = CenterOfScreenX - ( thePlayer->GetWidth( ) << 2 );
	rHook.top      = CenterOfScreenY - ( thePlayer->GetHeight( ) );
	rHook.right    = CenterOfScreenX + ( thePlayer->GetWidth( ) << 2 );
	rHook.bottom   = CenterOfScreenY + ( thePlayer->GetHeight( ) );

	if( !IntersectRect( &r, &rPlayer, &rCamera ) )
	{
		theCamera->SetCameraXOffset( thePlayer->GetPosX( ) - width / 2 );
		theCamera->SetCameraYOffset( thePlayer->GetPosY( ) - height / 2 );
	}

	if (IntersectRect( &r, &rHook, &rPlayer ) )
	{
		theCamera->SetVelocityX( theCamera->GetVelocityX( ) * 0.95f );
		theCamera->SetVelocityY( theCamera->GetVelocityY( ) * 0.95f );
	}

	{
		if( rPlayer.left < rHook.left && thePlayer->GetVelX() < -110)
			theCamera->SetVelocityX( thePlayer->GetVelX());
		else if( rPlayer.left < rHook.left )
			theCamera->SetVelocityX( -100 );
		else if( rPlayer.right > rHook.right && thePlayer->GetVelX() > 110)
			theCamera->SetVelocityX( thePlayer->GetVelX() );
		else if( rPlayer.right > rHook.right )
			theCamera->SetVelocityX( 100 );

		if( rPlayer.top < rHook.top )
			theCamera->SetVelocityY( -100 );
		else if( rPlayer.bottom > rHook.bottom && thePlayer->GetVelY() > 200)
			theCamera->SetVelocityY( thePlayer->GetVelY() );
		else if(rPlayer.bottom > rHook.bottom)
			theCamera->SetVelocityY( 200 );
		
	}

	theCamera->SetCameraXOffset( theCamera->GetXOffset( ) + theCamera->GetVelocityX( ) * fElapsedTime );
	theCamera->SetCameraYOffset( theCamera->GetYOffset( ) + theCamera->GetVelocityY( ) * fElapsedTime );
}