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
#if _DEBUG
	memcpy(m_szMapData[15], "**            **", 16 + 1);
	memcpy(m_szMapData[16], "**            **", 16 + 1);
	memcpy(m_szMapData[17], "**            **", 16 + 1);
	memcpy(m_szMapData[18], "**            **", 16 + 1);
	memcpy(m_szMapData[19], "**            **", 16 + 1);
	memcpy(m_szMapData[20], "**            **", 16 + 1);
	memcpy(m_szMapData[21], "**            **", 16 + 1);
	memcpy(m_szMapData[22], "**            **", 16 + 1);
#else
	memcpy(m_szMapData[15], "**            **", 16 + 1);
	memcpy(m_szMapData[16], "**            **", 16 + 1);
	memcpy(m_szMapData[17], "**            **", 16 + 1);
	memcpy(m_szMapData[18], "**            **", 16 + 1);
	memcpy(m_szMapData[19], "**            **", 16 + 1);
	memcpy(m_szMapData[20], "**            **", 16 + 1);
	memcpy(m_szMapData[21], "**            **", 16 + 1);
	memcpy(m_szMapData[22], "**            **", 16 + 1);
#endif
	memcpy(m_szMapData[23], "****************", 16 + 1);
	memcpy(m_szMapData[24], "****************", 16 + 1);
	memcpy(m_szMapData[25], "score : ", sizeof("score : 0"));
}

bool CMap::IsCollide(CTetrimino* pTetrimino)
{
	auto tBlkPositions = pTetrimino->GetBlockPositions();

	for (const auto& tBlkPosition : tBlkPositions) {
		//충돌 직전 체크
		/*int dx[] = {-1, 1, 0, 0};
		int dy[] = { 0, 0, -1 , 1 };

		for (int i = 0; i < 4; i++) {
			if (m_szMapData[tBlkPosition.y + dy[i]][tBlkPosition.x + dx[i]] == '*') {
				return true;
			}
		}*/

		if (m_szMapData[tBlkPosition.y][tBlkPosition.x] == '*')
			return true;
	}
	return false;
}


void CMap::Pile(CTetrimino* pTetrimino)
{
	// TODO
	auto tBlkPositions = pTetrimino->GetBlockPositions();

	for(const auto& tBlkPosition : tBlkPositions)
		this->m_szMapData[tBlkPosition.y][tBlkPosition.x] = '*';
}

void CMap::OnDraw(CRenderer* pRender)
{
	for (int i = 0; i < g_nMapHeight; i++)
		pRender->Draw(m_nPosX, m_nPosY + i, m_szMapData[i], g_nMapWidth);
}

//beomu
bool CMap::isCheckLineComplete(int row) {
	for (int col = 0; col < g_nMapWidth; col++)
		if (m_szMapData[row][col] != '*') return false;

	return true;
}

void CMap::removeLine(int row) {
	int size = (g_nMapWidth + 1) * (row);

	char* tmp = new char[size];
	memcpy(tmp, m_szMapData[0], size);

	for (int col = 2; col < g_nMapWidth - 2; col++) {
		m_szMapData[0][col] = ' ';
		m_szMapData[row][col] = ' ';
	}

	memcpy(m_szMapData[1], tmp, size);
	delete(tmp);
}

uint8_t CMap::CheckLineCompleteAndClear(CTetrimino* pTetrimino) {
	auto tBlkPositions = pTetrimino->GetBlockPositions();
	uint8_t ret{};

	for (const auto& tBlkPosition : tBlkPositions) {
		if (isCheckLineComplete(tBlkPosition.y)) {
			removeLine(tBlkPosition.y);	
			ret++;
		}
	}

	return ret;

}

void CMap::SetScore(UINT score) {
	std::string ret("score : ");
	ret.append(std::to_string(score));

	memcpy(m_szMapData[g_nMapHeight-1], ret.c_str(), ret.length()+1);
}
