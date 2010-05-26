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
		int NumAnims = 0;
		in.read((char*)& NumAnims,sizeof(int));

		for(int i = 0; i < NumAnims; i++)
		{
			CAnimation* anim = new CAnimation;
			vector<Frame*> fvec;
			int AnimNameLength = 0;
			in.read((char*)& AnimNameLength,sizeof(char));

			char* animationname = new char[AnimNameLength+1];
			in.read(animationname,sizeof(char)* AnimNameLength);
			delete[] animationname;

			int animationid = 0;
			in.read((char*) & animationid, sizeof(int));

			int animationtype;
			in.read((char*)&animationtype, sizeof(int));

			int PictureFileNameLength = 0;

			in.read((char*) &PictureFileNameLength, sizeof(char));

			char* PictureFileName = new char[PictureFileNameLength +1];

			in.read(PictureFileName, sizeof(char) * PictureFileNameLength);
			PictureFileName[PictureFileNameLength] =0;

			//load into texture manager and save id
			anim->SetImageID(CSGD_TextureManager::GetInstance()->LoadTexture(imagepath,keycolor));
			delete[] PictureFileName;

			bool looping;
			in.read((char*)&looping, sizeof(bool));
			anim->SetLooping(looping);

			int numframes = 0;
			in.read((char*) &numframes, sizeof(int));
			for(int i =0; i < numframes; i++)
			{
				Frame* newframe = new Frame;
				int framenum = 0;
				in.read((char*) &framenum, sizeof(int));

			
				in.read((char*) &newframe->WaitTime, sizeof(double));

				int triggerlength = 0;
				in.read((char*) &triggerlength, sizeof(char));

				newframe->Trigger = new char[triggerlength +1];
				in.read(newframe->Trigger, sizeof(char) * triggerlength);
				newframe->Trigger[triggerlength] = 0;

				

		
				int renderwidth = 0;
				int renderheight = 0;

				in.read((char*)&(newframe->DrawRect.left), sizeof(int));
				in.read((char*)&(newframe->DrawRect.top), sizeof(int));
				in.read((char*)&(renderwidth), sizeof(int));
				in.read((char*)&(renderheight), sizeof(int));

				newframe->DrawRect.right = newframe->DrawRect.left + renderwidth;
				newframe->DrawRect.bottom = newframe->DrawRect.top + renderheight;

				int collisionwidth = 0;
				int collisionheight = 0;
				in.read((char*)&(newframe->CollisionRect.left), sizeof(int));
				in.read((char*)&(newframe->CollisionRect.top), sizeof(int));
				in.read((char*)&(collisionwidth), sizeof(int));
				in.read((char*)&(collisionheight), sizeof(int));

				newframe->CollisionRect.right = newframe->CollisionRect.left + collisionwidth;
				newframe->CollisionRect.bottom = newframe->CollisionRect.top + collisionheight;

				in.read((char*)&newframe->AnchorPoint.x,sizeof(int));
				in.read((char*)&newframe->AnchorPoint.y, sizeof(int));
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
	LoadAnimationSet("resource/Jack.Anim", D3DCOLOR_XRGB(255, 255, 255),"resource/graphics/jack_96.png");

	LoadAnimationSet("resource/fireEnemy.Anim", D3DCOLOR_XRGB(255, 255, 255),"resource/graphics/Lapid_lulzfireenemy.png");
	LoadAnimationSet("resource/iceEnemy.Anim", D3DCOLOR_XRGB(127, 127, 127),"resource/graphics/ice_128.bmp");
	LoadAnimationSet("resource/windEnemy.Anim", D3DCOLOR_XRGB(195, 195, 195),"resource/graphics/bird.png");
	LoadAnimationSet("resource/earthEnemy.Anim", NULL ,"resource/graphics/Lapidem_lulzenemy.png");
	LoadAnimationSet("resource/docEnemy.Anim",D3DCOLOR_XRGB(255, 255, 255) ,"resource/graphics/DoctorBoss.png");

	LoadAnimationSet("resource/sisEnemy.Anim",D3DCOLOR_XRGB(0, 0, 0) ,"resource/graphics/Lapidem_WomanBoss.png");



}