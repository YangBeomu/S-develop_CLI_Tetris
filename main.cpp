#include "pch.h"
#include "Tetris.h"

int main()
{
	CTetris tetris;

	int nFPS = 10;
	int nSleepTime = 1000 / nFPS;
	while (GAME_ERROR != tetris.GetState() && GAME_END != tetris.GetState())
	{
		ST_KEY_STATE stKeyState;
		stKeyState.Clear();

		tetris.Input(&stKeyState);
		tetris.Update(stKeyState);
		tetris.Render();

		Sleep(nSleepTime);
	}

	system("cls");
	MessageBox(NULL, TEXT("게임 오버!"), TEXT("테트리스"), NULL);

	return 0;
}
