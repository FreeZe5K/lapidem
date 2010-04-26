#include "CCamera.h"
#include "CBase.h"

//#include "CGameplayState.h"

#define XHOOK_MIN_DISTANCE (int)theCamera->GetPlayerXhookDistance() >> 1
#define YHOOK_MIN_DISTANCE (int)theCamera->GetPlayerXhookDistance() >> 1

CCamera * CCamera::theCamera = 0;

void CCamera::InitCamera(float fPosX, float fPosY, float fWidth, float fHeight, float fXHook, float fYHook, CBase* ObjectToFollow)
{
	if(!theCamera)
	{
		theCamera = new CCamera();
		theCamera->SetCameraXOffset(fPosX);
		theCamera->SetCameraYOffset(fPosY);
		theCamera->SetCameraWidth(fWidth);
		theCamera->SetCameraHeight(fHeight);
		theCamera->SetPlayerHooks(fXHook, fYHook);
		theCamera->thePlayer = ObjectToFollow;
		theCamera->SetVelocityX(0.0f);
		theCamera->SetVelocityY(0.0f);
	}
}

void CCamera::ShutDownCamera()
{
	if(theCamera)
		delete theCamera;
}

void CCamera::Update(float fElapsedTime)
{
		//float DistanceX = thePlayer->GetPosX() - theCamera->GetXOffset(); 
		//float DistanceY = thePlayer->GetPosY() - theCamera->GetYOffset();

	int width = theCamera->GetWidth()-theCamera->GetXOffset();
	int height = theCamera->GetHeight()-theCamera->GetYOffset();/*
	theCamera->SetCameraXOffset( thePlayer->GetPosX() - (width/2));
	theCamera->SetCameraYOffset( thePlayer->GetPosY() - (height/2));*/


		//int fScreenCenterX = (int)width>>1;
		//int fScreenCenterY = (int)width>>1;
		RECT rCamera;
		rCamera.left = GetXOffset();
		rCamera.right = rCamera.left + GetWidth();
		rCamera.top = GetYOffset();
		rCamera.bottom = rCamera.top + GetHeight();

		

		RECT rPlayer = thePlayer->GetCollisionRect(fElapsedTime);
		//rPlayer.left -= 100;
		//rPlayer.right += 100;
		//rPlayer.top -= 100;
		//rPlayer.bottom += 100;

		int CenterOfScreenX = theCamera->GetXOffset() + width/2;
		int CenterOfScreenY = theCamera->GetYOffset() + height/2;

			RECT rHook ;//= { CenterOfScreenX - 100, CenterOfScreenY - 100, CenterOfScreenX+100, CenterOfScreenY+100 };

			
		rHook.left = CenterOfScreenX - 100;
		rHook.top = CenterOfScreenY - 50;
		rHook.right =CenterOfScreenX + 100;
		rHook.bottom = CenterOfScreenY + 50;
		RECT r;

		if(!IntersectRect(&r, &rPlayer, & rCamera ))
		{
			theCamera->SetCameraXOffset( thePlayer->GetPosX() - width/2 );
			theCamera->SetCameraYOffset( thePlayer->GetPosY() - height/2 );

		}

		if (IntersectRect( &r, &rHook, &rPlayer ))
		{
			theCamera->SetVelocityX(theCamera->GetVelocityX() * 0.95f );
			theCamera->SetVelocityY(theCamera->GetVelocityY() * 0.95f );
			
			
		}
		{
			if( rPlayer.left < rHook.left )
			theCamera->SetVelocityX(-100);
			else if( rPlayer.right > rHook.right )
			theCamera->SetVelocityX(100);

			if( rPlayer.top < rHook.top )
			theCamera->SetVelocityY(-100);
			else if( rPlayer.bottom > rHook.bottom)
			theCamera->SetVelocityY(200);


		}

		//
		////if(IntersectRect( &r, &rHook, &rPlayer ) )//r.left  != rPlayer.left ||r.right  != rPlayer.right||r.top  != rPlayer.top||r.bottom  != rPlayer.bottom )
		////{
		//	
		//if( rPlayer.left < rHook.left )//|| rHook.right > rCamera.right )
		//{
		//	//rCamera.left -= (5);
		//	theCamera->SetVelocityX(-10000 *fElapsedTime);
		//}
		//else if(  rPlayer.right > rHook.right )
		//{
		//	
		//	//rCamera.left += (5);
		//	theCamera->SetVelocityX(10000 *fElapsedTime);
		//}
		//
		//if( rPlayer.top < rHook.top )//|| rHook.bottom > rCamera.bottom )
		//{
		//	theCamera->SetVelocityY(10000*fElapsedTime);
		//}
		//else if( rPlayer.bottom > rHook.bottom )
		//{
		//	theCamera->SetVelocityY(-10000*fElapsedTime);
		//}


		////}
		//


		//if( rHook.left < rCamera.left )//|| rHook.right > rCamera.right )
		//	theCamera->SetVelocityX(-10000 *fElapsedTime);
		//if(  rHook.right > rCamera.right )
		//	theCamera->SetVelocityX(10000 *fElapsedTime);
		//
		//if( rHook.top < rCamera.top )//|| rHook.bottom > rCamera.bottom )
		//	theCamera->SetVelocityY(10000*fElapsedTime);
		//if( rHook.bottom > rCamera.bottom )
		//	theCamera->SetVelocityY(-10000*fElapsedTime);

		//if( theCamera->GetVelocityX() < 0.1f )
		//	theCamera->SetVelocityX( 0.0f );
		//if( theCamera->GetVelocityY() < 0.1f )
		//	theCamera->SetVelocityY( 0.0f );



	/*

		if(DistanceX > theCamera->GetPlayerXhookDistance() || DistanceX < XHOOK_MIN_DISTANCE)
			theCamera->SetVelocityX(thePlayer->GetVelX());
		else
			theCamera->SetVelocityX(0.0f);

		if(DistanceY > theCamera->GetPlayerXhookDistance() || DistanceY < YHOOK_MIN_DISTANCE)
			theCamera->SetVelocityY(thePlayer->GetVelY());
		else
			theCamera->SetVelocityY(0.0f);*/

		theCamera->SetCameraXOffset(theCamera->GetXOffset() + theCamera->GetVelocityX() * fElapsedTime);
		theCamera->SetCameraYOffset(theCamera->GetYOffset() + theCamera->GetVelocityY() * fElapsedTime);
	
			/*theCamera->SetCameraXOffset(rCamera.left);
		theCamera->SetCameraYOffset(rCamera.top );*/
	
	
}