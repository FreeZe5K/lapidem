#include "Corona_ObjectManager.h"
#include "CBase.h"

Corona_ObjectManager * Corona_ObjectManager::CoMReference = NULL;

Corona_ObjectManager* Corona_ObjectManager::GetInstance()
{
	if(!CoMReference)
		CoMReference = new Corona_ObjectManager();

	return CoMReference;
}

void Corona_ObjectManager::DeleteInstance()
{
	if(CoMReference)
	{
		RemoveAllObjects();
		delete this;
	}
}


void Corona_ObjectManager::UpdateObjects(float fElapsedTime)
{
	//TODO Check against camera to decide if an object needs to be updated.
	vector<CBase*> DeadItems;
	vector<CBase*>::iterator iter = Objects.begin();
	
	while(iter != Objects.end())
	{
		if((*iter)->IsActive())
			(*iter)->Update(fElapsedTime);
		else
			DeadItems.push_back(*iter);

		++iter;
	}

	iter = Terrain.begin();
	while(iter != Terrain.end())
	{
		if((*iter)->IsActive())
			(*iter)->Update(fElapsedTime);
		else
			DeadItems.push_back(*iter);

		++iter;
	}	

	for(unsigned index = 0; index < DeadItems.size(); ++index)
	{
		RemoveObject(DeadItems[index]);
	}

	//TODO Replace with a thread started in the constructor;
	CheckCollisions();
	
}

void Corona_ObjectManager::RenderObjects(void)
{
	//TODO Check against camera to decide if an object needs to be Rendered.
	for(unsigned index = 0; index < Objects.size(); ++index)
	{
		Objects[index]->Render();
	}

	for(unsigned index = 0; index < Terrain.size(); ++index)
	{
		Terrain[index]->Render();
	}
}

void Corona_ObjectManager::AddObject(CBase* ObjectToAdd)
{
	//Load Objects into the appropriate vector
	if(ObjectToAdd->GetType() != OBJ_TERRA)
		Objects.push_back(ObjectToAdd);
	else
		Terrain.push_back(ObjectToAdd);
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