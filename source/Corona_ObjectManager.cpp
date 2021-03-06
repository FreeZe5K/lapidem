#include "stdheaders.h"
#include "CBase.h"
#include "CTerrainBase.h"
#include "CProfiler.h"

//TODO Clean up "Crash Hunter" Items.
#include "CEnemy.h"

#define __PABLOS_COLLISION 

Corona_ObjectManager * Corona_ObjectManager::CoMReference = NULL;

Corona_ObjectManager* Corona_ObjectManager::GetInstance()
{
	if(!CoMReference)
	{
		CoMReference = new Corona_ObjectManager();
		GetInstance()->theCamera = CCamera::GetCamera();
		GetInstance()->Number = 0;
	}

	return CoMReference;
}

void Corona_ObjectManager::DeleteInstance()
{
	if(CoMReference)
	{
		RemoveAllObjects();
		theCamera->ShutDownCamera();
		delete CoMReference;
		CoMReference = NULL;
	}
}

void Corona_ObjectManager::UpdateObjects(float fElapsedTime)
{
#ifdef _DEBUG
	CProfiler::GetInstance()->Start("ObjectManager Update");
#endif

	vector<CBase*> DeadItems;

	for(unsigned index = 0; index < Objects.size(); ++index)
	{
			if(Objects[index]->IsActive() )
				Objects[index]->Update(fElapsedTime);
			else
				DeadItems.push_back(Objects[index]);
	}


	CheckCollisions(fElapsedTime);
	
	if( theCamera )
		theCamera->Update(fElapsedTime);

	m_fTimer = m_fTimer + fElapsedTime;

	if( m_fTimer >= 5 )
	{
		m_fTimer = 0.0f;
		Corona_EventHandler::GetInstance( )->SendEvent( "OffScreenPulse" );
	}

	for(unsigned index = 0; index < DeadItems.size(); ++index)
			RemoveObject(DeadItems[index]);


#ifdef _DEBUG
	CProfiler::GetInstance( )->End( "ObjectManager Update" );
#endif
}

void Corona_ObjectManager::RenderObjects(void)
{
#ifdef _DEBUG
	CProfiler::GetInstance()->Start("ObjectManager Render");
#endif
	for(unsigned index = 0; index < Objects.size(); ++index)
	{
		if(IsOnScreen(Objects[index]))
			Objects[index]->Render();
	}

#ifdef _DEBUG
	CProfiler::GetInstance()->End("ObjectManager Render");
#endif
}

void Corona_ObjectManager::AddObject(CBase* ObjectToAdd)
{
	Objects.push_back(ObjectToAdd);
	ObjectToAdd->AddRef();

	if(ObjectToAdd->GetType () == 2)
	{
		
		Object_Type CrashFinder;
		CrashFinder.eletype = ((CEnemy*)ObjectToAdd)->GetEleType();
		CrashFinder.Item = ObjectToAdd;
		CrashFinder.number = ++Number;
		CrashFinder.Type = ObjectToAdd->GetType();
		CrashHunter.push_back(CrashFinder);
	}

}

void Corona_ObjectManager::RemoveObject(CBase* ObjectToRemove)
{
	if(ObjectToRemove == NULL)
		return;

	for(vector<CBase*>::iterator iter = Objects.begin(); iter != Objects.end(); ++iter)
		if(*iter == ObjectToRemove)
		{
			(*iter)->Release();
			Objects.erase(iter);
			break;
		}



	//while(iter != Objects.end())
	//{
	//	if(*iter == ObjectToRemove)
	//	{
	//		(*iter)->Release();
	//		Objects.erase(iter);
	//		return;
	//	}
	//	++iter;
	//}
}

void Corona_ObjectManager::CheckCollisions(float fElapsedTime)
{
#ifdef _DEBUG
	CProfiler::GetInstance()->Start("ObjectManager CheckCollision");
#endif
	vector<CBase*> ObjectsOnScreen;

	for(unsigned index = 0; index < Objects.size(); ++index)
	{
		for(unsigned jay = (index + 1); jay < Objects.size(); ++jay)
		{

			if(Objects[index]->CheckCollision(fElapsedTime, Objects[jay]))
			{
				Objects[index]->HandleCollision(fElapsedTime, Objects[jay]);
				Objects[jay]->HandleCollision(fElapsedTime, Objects[index]);
			}
		}

		ObjectsOnScreen.push_back(Objects[index]);
	}

#ifdef  __PABLOS_COLLISION
	//////////////
	// Try at optimization
	//////////////
	for(unsigned jay = 0; jay < ObjectsOnScreen.size(); ++jay)
	{
		CBase* pBase = CGameplayState::GetInstance()->GetLevel()->CheckCollision( ObjectsOnScreen[jay]);
		while( pBase )
		{
			pBase->HandleCollision(fElapsedTime, ObjectsOnScreen[jay]);
			ObjectsOnScreen[jay]->HandleCollision(fElapsedTime, pBase );

			pBase = CGameplayState::GetInstance()->GetLevel()->CheckCollision( ObjectsOnScreen[jay] );
		}

	}
#else
	//TODO? Optimize Terrain collision check
	for(unsigned index = 0; index < Terrain.size(); ++index)
	{
		if(!IsOnScreen(Terrain[index]))
			continue;

		for(unsigned jay = 0; jay < ObjectsOnScreen.size(); ++jay)
		{
			if(Terrain[index]->CheckCollision(fElapsedTime, ObjectsOnScreen[jay]))
			{
				Terrain[index]->HandleCollision(ObjectsOnScreen[jay]);
				ObjectsOnScreen[jay]->HandleCollision(Terrain[index]);
			}
		}
	}

#endif
#ifdef _DEBUG
	CProfiler::GetInstance()->End("ObjectManager CheckCollision");
#endif
}

void Corona_ObjectManager::RemoveAllObjects(void)
{
#ifdef _DEBUG
	CProfiler::GetInstance()->Start("ObjectManager RemoveAllObjects");
#endif

	for( unsigned index = 0; index < Objects.size( ); ++index )
		Objects[index]->Release( );
	Objects.clear( );

#ifdef _DEBUG
	CProfiler::GetInstance()->End( "ObjectManager RemoveAllObjects" );
#endif
}

bool Corona_ObjectManager::IsOnScreen( CBase* Object )
{
	if( Object->GetPosX( ) + Object->GetWidth( ) >= theCamera->GetXOffset( ) 
		&& Object->GetPosX( ) <= theCamera->GetWidth( ) )
		if( Object->GetPosY( ) + Object->GetHeight( ) >= theCamera->GetYOffset( ) && 
			Object->GetPosY( ) <= theCamera->GetHeight( ) )
			return true;
	return false;
}

//////////////
// Function: AuxFunction
//
// Purpose: To allow a function to be called on all 
// types of a certain object in the manager.
//////////////

void Corona_ObjectManager::AuxFunction(void (*pFunc)(CBase* object, CBase* object2), int OBJ_TYPE, bool AllInManager, CBase* pSpell)
{
	if(AllInManager)
	{
		for(unsigned index = 0; index < Objects.size(); ++index)
		{
			if(Objects[index]->GetType() == OBJ_TYPE)
			{
				(*pFunc)(Objects[index], pSpell);
			}
		}

	}
	else
	{
		for(unsigned index = 0; index < Objects.size(); ++index)
		{
			if(Objects[index]->GetType() == OBJ_TYPE && IsOnScreen(Objects[index]))
			{
				(*pFunc)(Objects[index],pSpell);
			}
		}
	}
}