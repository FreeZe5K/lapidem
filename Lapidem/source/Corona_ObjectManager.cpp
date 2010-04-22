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
		if((*iter)->GetPosX() > theCamera->GetXOffset() && ((*iter)->GetPosX() < theCamera->GetWidth()))
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
		if((*iter)->GetPosX() > theCamera->GetXOffset() && ((*iter)->GetPosX() < theCamera->GetWidth()))
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

	CheckCollisions();
	
}

void Corona_ObjectManager::RenderObjects(void)
{
	for(unsigned index = 0; index < Objects.size(); ++index)
	{
		if(Objects[index]->GetPosX() > theCamera->GetXOffset() && Objects[index]->GetPosX() < theCamera->GetWidth())
			Objects[index]->Render();
	}

	for(unsigned index = 0; index < Terrain.size(); ++index)
	{
		if(Terrain[index]->GetPosX() > theCamera->GetXOffset() && Terrain[index]->GetPosX() < theCamera->GetWidth())
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


void Corona_ObjectManager::CheckCollisions()
{


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