#include "pch.h"
#include "Map.h"

void CMap::Clear(void)
{
	m_nPosX = 0;
	m_nPosY = 0;

	memcpy(m_szMapData[0],  "**            **", 16 + 1);
	memcpy(m_szMapData[1],  "**            **", 16 + 1);
	memcpy(m_szMapData[2],  "**            **", 16 + 1);
	memcpy(m_szMapData[3],  "**            **", 16 + 1);
	memcpy(m_szMapData[4],  "**            **", 16 + 1);
	memcpy(m_szMapData[5],  "**            **", 16 + 1);
	memcpy(m_szMapData[6],  "**            **", 16 + 1);
	memcpy(m_szMapData[7],  "**            **", 16 + 1);
	memcpy(m_szMapData[8],  "**            **", 16 + 1);
	memcpy(m_szMapData[9],  "**            **", 16 + 1);
	memcpy(m_szMapData[10], "**            **", 16 + 1);
	memcpy(m_szMapData[11], "**            **", 16 + 1);
	memcpy(m_szMapData[12], "**            **", 16 + 1);
	memcpy(m_szMapData[13], "**            **", 16 + 1);
	memcpy(m_szMapData[14], "**            **", 16 + 1);
	memcpy(m_szMapData[15], "**            **", 16 + 1);
	memcpy(m_szMapData[16], "**            **", 16 + 1);
	memcpy(m_szMapData[17], "**            **", 16 + 1);
	memcpy(m_szMapData[18], "****************", 16 + 1);
	memcpy(m_szMapData[19], "****************", 16 + 1);
}

bool CMap::IsCollide(CTetrimino* pTetrimino)
{
	return false;
}

void CMap::Pile(CTetrimino* pTetrimino)
{
	// TODO
}

void CMap::OnDraw(CRenderer* pRender)
{
	for (int i = 0; i < g_nMapHeight; i++)
		pRender->Draw(m_nPosX, m_nPosY + i, m_szMapData[i], g_nMapWidth);
}