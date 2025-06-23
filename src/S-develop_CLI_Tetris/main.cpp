#include "pch.h"
#include "Tetris.h"

// 일시 정지 화면 표시 함수
void PauseScreen()
{
	// 콘솔 핸들 가져오기
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	// 화면 지우기
	system("cls");

	// 콘솔 커서 숨기기
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(hConsole, &cursorInfo);
	cursorInfo.bVisible = false;
	SetConsoleCursorInfo(hConsole, &cursorInfo);

	// PAUSE 메시지 출력
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
	printf("\n\n");
	printf(" _____                      \n");
	printf("|  __ \\                     \n");
	printf("| |__) |_ _ _   _ ___  ___ \n");
	printf("|  ___/ _` | | | / __|/ _ \\\n");
	printf("| |  | (_| | |_| \\__ \\  __/\n");
	printf("|_|   \\__,_|\\__,_|___/\\___|\n");
	printf("\n\n");

	// 경계선과 메시지 출력
	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	printf("======================================\n");

	// 게임 계속 안내
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	printf("[Press any key to continue...]\n");

	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	printf("======================================\n\n");

	// 콘트롤 안내
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	printf("Controls: %s%s Move   %s Rotate   Spacebar: Drop\n",
		"\xE2\x86\x90", "\xE2\x86\x92", "\xE2\x86\x91");

	// 원래 색상으로 복원
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	// 키 입력 대기 (반환 값 처리)
	int ch = getchar();
	(void)ch; // 반환 값 사용하여 경고 제거

	// 커서 다시 보이게 설정
	cursorInfo.bVisible = true;
	SetConsoleCursorInfo(hConsole, &cursorInfo);

	// 화면 지우기
	system("cls");
}

void StartScreen()
{
	// 콘솔 핸들 가져오기
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	// 화면 지우기
	system("cls");

	// 콘솔 커서 숨기기
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(hConsole, &cursorInfo);
	cursorInfo.bVisible = false;
	SetConsoleCursorInfo(hConsole, &cursorInfo);

	// TETRIS 로고 출력 (ASCII 아트)
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	printf("\n\n");
	printf(" _____    _        _     \n");
	printf("|_   _|__| |_ _ __(_)___ \n");
	printf("  | |/ _ \\ __| '__| / __|\n");
	printf("  | |  __/ |_| |  | \\__ \\\n");
	printf("  |_|\\___|\\__|_|  |_|___/\n");
	printf("\n\n");

	// 경계선과 메시지 출력
	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	printf("======================================\n");

	// 저자 정보
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	printf("Tetris Game by Beomu\n\n");

	// 게임 시작 안내
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	printf("[Press any key to start...]\n");

	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	printf("======================================\n\n");

	// 콘트롤 안내
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	printf("Controls: %s%s Move   %s Rotate   Spacebar: Drop\n",
		"\xE2\x86\x90", "\xE2\x86\x92", "\xE2\x86\x91");
	printf("ESC: Exit Game   P: Pause Game\n");

	// 원래 색상으로 복원
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	// 키 입력 대기
	getchar();

	// 커서 다시 보이게 설정
	cursorInfo.bVisible = true;
	SetConsoleCursorInfo(hConsole, &cursorInfo);

	// 화면 지우기
	system("cls");
}

int main()
{
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);


	try {
		Client ci;

#ifndef _DEBUG
		AntiCheat ac;
#endif

		//if (ci.Connect()) return -1;

		bool restart = true;

		while (restart) {
			StartScreen();

			CTetris tetris;

			int nFPS = 60;
			int nSleepTime = 10000 / nFPS;

			HWND console = GetConsoleWindow();
			console = GetWindow(console, GW_OWNER);

			while (GAME_ERROR != tetris.GetState() && GAME_END != tetris.GetState())
			{
				if (console != GetForegroundWindow()) {
					PauseScreen();
					continue;
				}
				ST_KEY_STATE stKeyState;
				stKeyState.Clear();

				tetris.Input(&stKeyState);
				tetris.Update(stKeyState);
				tetris.Render();

				Sleep(nSleepTime);
			}

			system("cls");

			// 게임 오버 메시지와 함께 재시작 옵션 제공
			int response = MessageBox(NULL, TEXT("게임 오버! 다시 시작하시겠습니까?"),
				TEXT("테트리스"), MB_YESNO | MB_ICONQUESTION);

			// 사용자 선택에 따라 재시작 여부 결정
			restart = (response == IDYES);
		}
	}
	catch (const std::exception& e) {
		std::cerr << "[main] " << e.what() << std::endl;
		return -1;
	}
}
