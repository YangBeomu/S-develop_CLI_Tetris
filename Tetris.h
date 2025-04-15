#pragma once

#include "Renderer.h"
#include "Tetrimino.h"
#include "Map.h"

struct ST_KEY_STATE
{
	bool bRotateKeyDown;
	bool bLeftKeyDown;
	bool bRightKeyDown;

	void Clear(void)
	{
		bRotateKeyDown = false;
		bLeftKeyDown = false;
		bRightKeyDown = false;
	}
};

class CTetris
{
	CRenderer m_Render;
	CTetrimino m_Tetrimino;
	CMap m_Map;

public:
	CTetris(void);
	~CTetris(void);

	void Input(ST_KEY_STATE* pKeyState);
	void Update(ST_KEY_STATE stKeyState);
	void Render(void);
};

