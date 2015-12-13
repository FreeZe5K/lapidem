#include "CIceCube.h"
#include "stdheaders.h"

#include "CEnemy.h"
#include "CIce.h"
#include "CEmitter.h"
#include "Lapidem_Math.h"

CIceCube::CIceCube()
{
	m_nRadius = 3;
	m_bExploded = false;
	this->SetType(OBJ_SPELL);
	this->SetElement(OBJ_ICE);
}

CIceCube::~CIceCube()
{
	CParticleManager::GetInstance()->RemoveEmitter(m_pEmitter);
}

void CIceCube::HandleCollision(float fElapsedTime, CBase* pObject)
{
	if(pObject->GetType() < 0)
		return;

	if( pObject->GetType( ) == OBJ_TERRA || pObject->GetType( ) ==  OBJ_ENEMY  )
	{
		CreateExplosion();
		m_bExploded = true;
	}
}

void CIceCube::Update(float fElapsedTime)
{
	this->SetVelY(this->GetVelY() + 100.0f * fElapsedTime);
	CSpell::Update(fElapsedTime);

	if(m_pEmitter)
	{
		m_pEmitter->SetPosX(GetPosX());
		m_pEmitter->SetPosY(GetPosY());
	}

	if(this->GetLifespan() <= 0.3f && !m_bExploded)
	{
		CreateExplosion();
		m_bExploded = true;
	}
}

void CIceCube::Render()
{
	CCamera* pCamera = CCamera::GetCamera();
	if(!m_bExploded)
	{
		CSGD_TextureManager::GetInstance()->Draw(this->GetImage(), (int)(this->GetPosX() - pCamera->GetXOffset()), (int)(this->GetPosY() - pCamera->GetYOffset()),
													1.0f, 1.0f, 0, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 255));
	}
}

void CIceCube::CreateExplosion()
{
	//CLevel* pLevel = CGameplayState::GetInstance()->GetLevel();

	//CTerrainBase* pTerrain;

	//for(int i = -m_nRadius; i < m_nRadius; ++i)
	//{
	//	for(int j = -m_nRadius; j < m_nRadius; ++j)
	//	{
	//		pTerrain = (CTerrainBase*)pLevel->GetTile((int)this->GetPosX() + i * pLevel->GetTileWidth(), (int)this->GetPosY() + j * pLevel->GetTileHeight());
	//		if(pTerrain->GetTypeTerrain() == T_ROCK)
	//		{
	//			pTerrain->SetTileID(pTerrain->GetBaseTile());
	//			pTerrain->SetTypeTerrain(T_EMPTY);
	//			pTerrain->SetActive(false);
	//		}
	//	}
	//}

	for(int i = 0; i < 10; ++i)
	{
		CreateSparks((D3DX_PI * 0.2f) * i);
	}

	this->SetActive(false);
}

void CIceCube::CreateSparks(float fDirRotation)
{
	tVector2D velocity;
	velocity._x = 0.0f; velocity._y = -1.0f;
	velocity = Lapidem_Math::GetInstance()->Vector2DRotate(velocity, fDirRotation);

	// - - - - - - - -
	// REPLACE
	// - - - - - - - - - - - - - - 
	//CEmitter* pEmitter = CEmitterFactory::GetInstance()->CreateEmitter("icespell");
	//pEmitter->SetPosX(this->GetPosX());
	//pEmitter->SetPosY(this->GetPosY());
	//pEmitter->SetVelX(250.0f * velocity._x);
	//pEmitter->SetVelY(250.0f * velocity._y);

	CIce* newfire = new CIce;

	
	CEmitter* hahaiworknow = CParticleManager::GetInstance()->LoadEmitter("resource/data/iceSpell.lapipt",0,0);
	newfire->SetEmitter(hahaiworknow);
	CParticleManager::GetInstance()->AddEmitter(newfire->GetEmitter());

	newfire->SetDamage(this->GetDamage());
	newfire->SetLifespan(0.5f);
	newfire->SetActive(true);
	newfire->SetTier(1);
	newfire->ShotBy(true);
	newfire->SetElement(OBJ_ICE);
	//newfire->SetEmitter(pEmitter);

	newfire->SetWidth(32);
	newfire->SetHeight(16);

	newfire->SetPosX(this->GetPosX());
	newfire->SetPosY(this->GetPosY());
	newfire->SetVelX(250.0f * velocity._x);
	newfire->SetVelY(250.0f * velocity._y);

	//CParticleManager::GetInstance()->AddEmitter(pEmitter);
	Corona_ObjectManager::GetInstance()->AddObject(newfire);
	newfire->Release();
}