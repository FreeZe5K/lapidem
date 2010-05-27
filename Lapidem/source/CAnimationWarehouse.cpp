#include "CAnimationWarehouse.h"
#include "CAnimation.h"
#include "Wrappers/CSGD_TextureManager.h"

#include <fstream>
using namespace std;
CAnimationWarehouse* CAnimationWarehouse::AnimationWarehouse = NULL;

void CAnimationWarehouse::DeleteInstance(void)
	{
		if(AnimationWarehouse)
		{
			for(unsigned int i =0; i <m_vAnimationList.size(); i++)
			{
				for(unsigned int j=0; j <m_vAnimationList[i].size(); j++)
				{
					for(unsigned int ftw =0; ftw < m_vAnimationList[i][j]->GetAllFrames().size(); ftw++)
					{
						delete m_vAnimationList[i][j]->GetAllFrames()[ftw]->Trigger;
						delete m_vAnimationList[i][j]->GetAllFrames()[ftw];
					}
					delete m_vAnimationList[i][j];
				}
			}
			delete AnimationWarehouse;
			AnimationWarehouse = 0;
		}
	}

void CAnimationWarehouse::LoadAnimationSet(char* Filename, DWORD keycolor, char* imagepath)
{
	ifstream in;
	in.open(Filename,ios_base::binary);
	if(in.is_open())
	{
		vector<CAnimation*> animations;
		int imagelength= 0;
		in.read((char*)&imagelength, sizeof(char));

		char* imagename = new char[imagelength+1];
		in.read(imagename,sizeof(char)* imagelength);
		delete[] imagename;

		int NumAnims = 0;
		in.read((char*)& NumAnims,sizeof(int));

		for(int i = 0; i < NumAnims; i++)
		{
			CAnimation* anim = new CAnimation;
			vector<Frame*> fvec;
			anim->SetImageID(CSGD_TextureManager::GetInstance()->LoadTexture(imagepath,keycolor));


			int AnimNameLength = 0;
			in.read((char*)& AnimNameLength,sizeof(char));

			char* animationname = new char[AnimNameLength+1];
			in.read(animationname,sizeof(char)* AnimNameLength);
			delete[] animationname;

			int numframes = 0;
			in.read((char*) &numframes, sizeof(int));
			for(int i =0; i < numframes; i++)
			{
				Frame* newframe = new Frame;
				

				in.read((char*)&(newframe->DrawRect.left), sizeof(int));
				in.read((char*)&(newframe->DrawRect.top), sizeof(int));
				in.read((char*)&(newframe->DrawRect.right), sizeof(int));
				in.read((char*)&(newframe->DrawRect.bottom), sizeof(int));

				in.read((char*)&(newframe->CollisionRect.left), sizeof(int));
				in.read((char*)&(newframe->CollisionRect.top), sizeof(int));
				in.read((char*)&newframe->CollisionRect.right, sizeof(int));
				in.read((char*)&newframe->CollisionRect.bottom, sizeof(int));

				in.read((char*)&newframe->AnchorPoint.x,sizeof(int));
				in.read((char*)&newframe->AnchorPoint.y, sizeof(int));

				in.read((char*) &newframe->WaitTime, sizeof(double));

				int triggerlength = 0;
				in.read((char*) &triggerlength, sizeof(char));

				newframe->Trigger = new char[triggerlength +1];
				in.read(newframe->Trigger, sizeof(char) * triggerlength);
				newframe->Trigger[triggerlength] = 0;

				fvec.push_back(newframe);
			}
			anim->SetFrames(fvec);
			animations.push_back(anim);
		}
		m_vAnimationList.push_back(animations);
	}

}

CAnimation* CAnimationWarehouse::GetAnimation(int nAnimationSet, int nAnimation)
{
	return m_vAnimationList[nAnimationSet][nAnimation];
}

void CAnimationWarehouse::Init(void)
{
	LoadAnimationSet("resource/data/Jack2.Anim", D3DCOLOR_XRGB(255, 255, 255),"resource/graphics/jack_96.png");
	LoadAnimationSet("resource/data/fireEnemy2.Anim", D3DCOLOR_XRGB(255, 255, 255),"resource/graphics/fire_128.png");
	LoadAnimationSet("resource/data/iceEnemy2.Anim", D3DCOLOR_XRGB(255, 255, 255),"resource/graphics/ice_128.png");
	LoadAnimationSet("resource/data/windEnemy2.Anim", D3DCOLOR_XRGB(255, 255, 255),"resource/graphics/bird.png");
	LoadAnimationSet("resource/data/Enemy.anim", D3DCOLOR_XRGB(255, 255, 255) ,"resource/graphics/rock_128.png");

	LoadAnimationSet("resource/data/sisEnemy.Anim",D3DCOLOR_XRGB(128, 128, 255) ,"resource/graphics/MiH_BlondeBarbie.png");
}