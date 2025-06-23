#pragma once

#include "RenderObject.h"
#include "Tetrimino.h"

const int g_nMapWidth = 16;
const int g_nMapHeight = 26;

class CMap : public CRenderObject
{
	char m_szMapData[g_nMapHeight][g_nMapWidth + 1];

	//beomu
	bool isCheckLineComplete(int row);
	void removeLine(int row);
	
public:
	void Clear(void);
	bool IsCollide(CTetrimino* pTetrimino);
	void Pile(CTetrimino* pTetrimino);
	void OnDraw(CRenderer* pRender);

	//beomu
	uint8_t CheckLineCompleteAndClear(CTetrimino* pTetrimino);
	void SetScore(UINT score);
};

