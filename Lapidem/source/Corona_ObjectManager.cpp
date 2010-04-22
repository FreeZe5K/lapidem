#include "Corona_ObjectManager.h"
#include "CCamera.h"
#include "CBase.h"

Corona_ObjectManager * Corona_ObjectManager::CoMReference = NULL;

Corona_ObjectManager* Corona_ObjectManager::GetInstance()
{
	if(!CoMReference)
	{
		CoMReference = new Corona_ObjectManager();
		GetInstance()->theCamera = CCamera::GetCamera();
	}

	return CoMReference;
}

void Corona_ObjectManager::DeleteInstance()
{
	if(CoMReference)
	{
		RemoveAllObjects();
		theCamera->ShutDownCamera();
		delete this;
	}
}


void Corona_ObjectManager::UpdateObjects(float fElapsedTime)
{
	vector<CBase*> DeadItems;
	vector<CBase*>::iterator iter = Objects.begin();
	
	while(iter != Objects.end())
	{
		if(IsOnScreen(*iter))
		{
			if((*iter)->IsActive())
				(*iter)->Update(fElapsedTime);
			else
				DeadItems.push_back(*iter);
		}

		++iter;
	}

	iter = Terrain.begin();
	while(iter != Terrain.end())
	{
		if(IsOnScreen(*iter))
		{
			if((*iter)->IsActive())
				(*iter)->Update(fElapsedTime);
			else
				DeadItems.push_back(*iter);
		}

		++iter;
	}	

	for(unsigned index = 0; index < DeadItems.size(); ++index)
	{
		RemoveObject(DeadItems[index]);
	}

	CheckCollisions(fElapsedTime);
	
}

void Corona_ObjectManager::RenderObjects(void)
{
	for(unsigned index = 0; index < Objects.size(); ++index)
	{
		if(IsOnScreen(Objects[index]))
			Objects[index]->Render();
	}

	for(unsigned index = 0; index < Terrain.size(); ++index)
	{
		if(IsOnScreen(Terrain[index]))
			Terrain[index]->Render();
	}
}

void Corona_ObjectManager::AddObject(CBase* ObjectToAdd)
{
	if(ObjectToAdd->GetType() != OBJ_TERRA)
		Objects.push_back(ObjectToAdd);
	else
		Terrain.push_back(ObjectToAdd);

	ObjectToAdd->AddRef();
}

void Corona_ObjectManager::RemoveObject(CBase* ObjectToRemove)
{
	if(ObjectToRemove == NULL)
		return;

	vector<CBase*>::iterator iter = Objects.begin();
	
	while(iter != Objects.end())
	{
		if(*iter == ObjectToRemove)
		{
			(*iter)->Release();
			Objects.erase(iter);
			return;
		}
		++iter;
	}
	
	iter = Terrain.begin();

	while(iter != Terrain.end())
	{
		if(*iter == ObjectToRemove)
		{
			(*iter)->Release();
			Terrain.erase(iter);
			return;
		}
		++iter;
	}
}


void Corona_ObjectManager::CheckCollisions(float fElapsedTime)
{
	vector<CBase*> ObjectsOnScreen;

	for(unsigned index = 0; index < Objects.size(); ++index)
	{
		if(!IsOnScreen(Objects[index]))
			continue;

		for(unsigned jay = (index + 1); jay < Objects.size(); ++jay)
		{
			if(!IsOnScreen(Objects[jay]))
				continue;
			
			if(Objects[index]->CheckCollision(fElapsedTime, Objects[jay]))
			{
				Objects[index]->HandleCollision(Objects[jay]);
				Objects[jay]->HandleCollision(Objects[index]);
			}

			ObjectsOnScreen.push_back(Objects[index]);
		}
	}

	for(unsigned index = 0; index < ObjectsOnScreen.size(); ++index)
	{
			//Check the Objects on the screen's location against the terrain.


	}
}

void Corona_ObjectManager::RemoveAllObjects(void)
{
	for(unsigned index = 0; index < Objects.size(); ++index)
	{
		Objects[index]->Release();
	}
	for(unsigned index = 0; index < Terrain.size(); ++index)
	{
		Terrain[index]->Release();
	}
	Objects.clear();
	Terrain.clear();
}
bool Corona_ObjectManager::IsOnScreen(CBase* Object)
{
	if(Object->GetPosX() > theCamera->GetXOffset() && Object->GetPosX() < theCamera->GetWidth())
		if(Object->GetPosY() > theCamera->GetYOffset() && Object->GetPosY() < theCamera->GetHeight())
			return true;


	return false;
}