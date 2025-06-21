#pragma once

#include "Renderer.h"
#include "Tetrimino.h"
#include "Map.h"
#include "Client.h"
#include "AntiDebug.h"
#include "AntiCheat.h"

struct ST_KEY_STATE
{
	bool bRotateKeyDown;
	bool bLeftKeyDown;
	bool bRightKeyDown;
	bool bSpaceKeyDown;
	bool& bSKeyDown = bSpaceKeyDown;


	void Clear(void)
	{
		bRotateKeyDown = false;
		bLeftKeyDown = false;
		bRightKeyDown = false;
		bSpaceKeyDown = false;
	}
};

//beomu
enum GAME_STATE {
	GAME_ERROR,
	GAME_INIT,
	GAME_PROGRESS,
	GAME_END
};

class CTetris
{
	CRenderer m_Render;
	CTetrimino m_Tetrimino;
	CMap m_Map;

	GAME_STATE m_State;

	//beomu
	static constexpr float ADD_SCORE_NUMBER = 100;
	std::list<uint8_t> score_{};
	float screenScore_{};

	void AddScore(const uint8_t& clearLine);
	float GetScore();

public:
	CTetris(void);
	~CTetris(void);

	void Input(ST_KEY_STATE* pKeyState);
	void Update(ST_KEY_STATE stKeyState);
	void Render(void);


	GAME_STATE GetState();
};

