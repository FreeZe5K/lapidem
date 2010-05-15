#include "CGrenade.h"
#include "CParticleManager.h"
#include "Wrappers/CSGD_TextureManager.h"
#include "CGameplayState.h"
#include "CEmitter.h"
#include "CParticleManager.h"
#include "CSpellFactory.h"
#include "Corona_ObjectManager.h"
#include "CEnemy.h"
#include "CFire.h"

#include "Lapidem_Math.h"

CGrenade::CGrenade()
{
	m_nRadius = 3;
	m_bExploded = false;
	this->SetType(OBJ_SPELL);
	this->SetElement(OBJ_FIRE);
}

CGrenade::~CGrenade()
{
	CParticleManager::GetInstance()->RemoveEmitter(m_pEmitter);
}

void CGrenade::HandleCollision(float fElapsedTime, CBase* pObject)
{
	if( pObject->GetType( ) == OBJ_TERRA || pObject->GetType( ) ==  OBJ_PLAYER  )
	{
		if( pObject->GetPosX( ) + 1 > GetPosX( ) + GetWidth( ) || 
			pObject->GetPosX( ) + GetWidth( ) - 1 < GetPosX( ) )
			SetVelX(GetVelX( ) * -0.5f );

		if( pObject->GetPosY( ) + GetHeight( ) - 1 < GetPosY( ) )
			SetVelY(GetVelX( ) * -0.4f );

		if( pObject->GetPosY( ) + 1 > GetPosY( ) + GetHeight( ) )
			SetVelY( GetVelY( ) * -0.2f );

		if(dynamic_cast<CTerrainBase*>(pObject))
		{
			if(((CTerrainBase*)pObject)->GetTypeTerrain() == T_BOUNDARY)
			{
				MoveOutOf(pObject, fElapsedTime);
				SetVelX(-GetVelX());
			}
		}
	}
}

void CGrenade::Update(float fElapsedTime)
{
	this->SetVelY(this->GetVelY() + 100.0f * fElapsedTime);
	CSpell::Update(fElapsedTime);
	if(m_pEmitter)
	{
		m_pEmitter->SetPosX(GetPosX());
		m_pEmitter->SetPosY(GetPosY());
	}

	if(this->GetLifespan() <= 0.5f && !m_bExploded)
	{
		CreateExplosion();
		m_bExploded = true;
	}
}

void CGrenade::Render()
{
	CCamera* pCamera = CCamera::GetCamera();
	if(!m_bExploded)
	{
		CSGD_TextureManager::GetInstance()->Draw(this->GetImage(), 
			(int)(this->GetPosX() - pCamera->GetXOffset()), (int)(this->GetPosY() - pCamera->GetYOffset()),
			1.0f, 1.0f, 0, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(255, 0, 0));
	}
}

void CGrenade::CreateExplosion()
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

void CGrenade::CreateSparks(float fDirRotation)
{
	tVector2D velocity;
	velocity._x = 0.0f; velocity._y = -1.0f;
	velocity = Lapidem_Math::GetInstance()->Vector2DRotate(velocity, fDirRotation);

	// - - - - - - - -
	// REPLACE
	// - - - - - - - - - - - - - - 
	//CEmitter* pEmitter = CEmitterFactory::GetInstance()->CreateEmitter("firespell");
	//pEmitter->SetPosX(this->GetPosX());
	//pEmitter->SetPosY(this->GetPosY());
	//pEmitter->SetVelX(250.0f * velocity._x);
	//pEmitter->SetVelY(250.0f * velocity._y);

	

	CFire* newfire = new CFire;
	
	
	CEmitter* hahaiworknow = CParticleManager::GetInstance()->LoadEmitter("resource/data/fireSpell.lapipt",0,0);
	newfire->SetEmitter(hahaiworknow);
	CParticleManager::GetInstance()->AddEmitter(newfire->GetEmitter());
	
	newfire->SetDamage(this->GetDamage());
	newfire->SetDOT(3);
	newfire->SetLifespan(0.5f);
	newfire->SetActive(true);
	newfire->SetTier(1);
	newfire->ShotBy(true);
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