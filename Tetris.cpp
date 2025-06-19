#include "pch.h"
#include "Tetris.h"

CTetris::CTetris(void)
{
	m_Tetrimino.Reset(rand() % TETRIMINO_COUNT);
	m_Map.Clear();
	
	//beomu
	m_State = GAME_PROGRESS;
}

CTetris::~CTetris(void)
{
}

void CTetris::Input(ST_KEY_STATE* pKeyState)
{
	int cKeyDown = 0;
	if (_kbhit()) {
		cKeyDown = _getch();
		if (cKeyDown == 0 || cKeyDown == 224) {  // 화살표 키는 확장 키로 처리됨
			cKeyDown = _getch();  // 두 번째 바이트 읽기
			switch (cKeyDown) {
			case 75:  // 왼쪽 화살표
				pKeyState->bLeftKeyDown = true;
				break;
			case 77:  // 오른쪽 화살표
				pKeyState->bRightKeyDown = true;
				break;
			case 72:  // 위쪽 화살표
				pKeyState->bRotateKeyDown = true;
				break;
			}
		}
		else {  // 기존 WASD 키 유지 (선택 사항)
			if ('A' == cKeyDown || 'a' == cKeyDown)
				pKeyState->bLeftKeyDown = true;
			if ('D' == cKeyDown || 'd' == cKeyDown)
				pKeyState->bRightKeyDown = true;
			if ('W' == cKeyDown || 'w' == cKeyDown)
				pKeyState->bRotateKeyDown = true;
		}
	}
}

void CTetris::Update(ST_KEY_STATE stKeyState)
{
	// TODO
	if (GAME_PROGRESS != m_State) return;

	if (stKeyState.bLeftKeyDown) {
		m_Tetrimino.Left();
		if (m_Map.IsCollide(&m_Tetrimino)) m_Tetrimino.Right();
	}
	else if (stKeyState.bRightKeyDown) {
		m_Tetrimino.Right();
		if (m_Map.IsCollide(&m_Tetrimino)) m_Tetrimino.Left();
	}
	else if (stKeyState.bRotateKeyDown) {
		do {
			m_Tetrimino.Rotate();
		} while (m_Map.IsCollide(&m_Tetrimino));
	}

	m_Tetrimino.Down();
	if (m_Map.IsCollide(&m_Tetrimino)) {
		m_Tetrimino.Up();

		m_Map.Pile(&m_Tetrimino);
		if (m_Map.CheckLineCompleteAndClear(&m_Tetrimino)) {
			score_ += ADD_SCORE_NUMBER;
			m_Map.SetScore(score_);
		}
		m_Tetrimino.Reset(rand() % TETRIMINO_COUNT);
		if (m_Map.IsCollide(&m_Tetrimino)) m_State = GAME_END;
	}
}

void CTetris::Render(void)
{
	m_Render.Clear();
	m_Map.OnDraw(&m_Render);
	m_Tetrimino.OnDraw(&m_Render);
	m_Render.Render();
}

//beomu
GAME_STATE CTetris::GetState() {
	return this->m_State;
}