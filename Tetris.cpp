#include "pch.h"
#include "Tetris.h"

CTetris::CTetris(void)
{
    m_Tetrimino.Reset(rand() % TETRIMINO_COUNT);
    m_Map.Clear();
}

CTetris::~CTetris(void)
{
}

void CTetris::Input(ST_KEY_STATE* pKeyState)
{
    char cKeyDown = 0;
    while (_kbhit())
        cKeyDown = _getch();

    if ('A' == cKeyDown || 'a' == cKeyDown)
        pKeyState->bLeftKeyDown = true;
    if ('D' == cKeyDown || 'd' == cKeyDown)
        pKeyState->bRightKeyDown = true;
    if ('W' == cKeyDown || 'w' == cKeyDown)
        pKeyState->bRotateKeyDown = true;
}

void CTetris::Update(ST_KEY_STATE stKeyState)
{
    // TODO
}

void CTetris::Render(void)
{
    m_Render.Clear();
    m_Map.OnDraw(&m_Render);
    m_Tetrimino.OnDraw(&m_Render);
    m_Render.Render();
}