#pragma once
#include "Tetris.h"

class AntiCheat
{
private:
	Client ci;
	AntiDebug ad;
	
	DWORD GetProcessID(std::vector<uint8_t>& processName);

protected:

public:
	static constexpr uint32_t PROCESS_LIST_NUM = 1024;

	AntiCheat();
};