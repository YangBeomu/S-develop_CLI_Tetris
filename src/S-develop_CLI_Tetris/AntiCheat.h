#pragma once
#include "Tetris.h"

class AntiCheat
{
private:
	AntiDebug ad;

	//remote process variables
	uint16_t rProcessId_{};

	void InitRemoteProcess();
	uint16_t GetProcessID(std::vector<uint8_t>& processName);

protected:

public:
	static constexpr uint32_t PROCESS_LIST_NUM = 1024;

	explicit AntiCheat();
	~AntiCheat();
};