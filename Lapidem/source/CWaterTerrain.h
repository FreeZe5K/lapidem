#pragma once

#include "CTerrainBase.h"
//#include "CLDevice.h"

class CWaterTerrain : public CTerrainBase//, public CLDevice
{
	int m_nDistanceToFlood;
	int m_nCurrDistance;

	bool m_bIsFlooding;

	float m_fFloodSpeed;
	float m_fCurrTime;

	bool m_bLeftUnFloodable, m_bRightUnFloodable, m_bTopUnFloodable;
	int m_nNumFlooded;

	int m_nWaterTileID;
	CTerrainBase* m_pControlTile;

public:
	CWaterTerrain();
	~CWaterTerrain();

	void SetDistance(int nDistance) { m_nDistanceToFlood = nDistance; }
	void SetFloodSpeed(float fSpeed) { m_fFloodSpeed = fSpeed; }
	void SetFlood(bool bIsFlooding) { m_bIsFlooding = bIsFlooding; }
	void SetWaterID(int nTileID) { m_nWaterTileID = nTileID; }
	void SetControlTile(CTerrainBase* pTile) { m_pControlTile = pTile; }

	int GetTotalDistance() { return m_nDistanceToFlood; }
	int GetCurrDistance() { return m_nCurrDistance; }
	float GetFloodSpeed() { return m_fFloodSpeed; }
	bool GetFlood() { return m_bIsFlooding; }
	CTerrainBase* GetControlTile() { return m_pControlTile; }

	void HandleCollision(CBase*);
	//void HandleEvent(CEvent* pEvent);

	void Update(float);
	//void Render();
};