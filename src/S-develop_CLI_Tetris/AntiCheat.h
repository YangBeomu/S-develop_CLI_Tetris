#pragma once
#include "Tetris.h"

class AntiCheat
{
protected:
	enum status {
		STATUS_IDLE = 0,
		STATUS_RUNNING = 1,
		STATUS_STOPPED = 2,
		STATUS_ENDED = 3,
		STATUS_ERROR = 4,
		STATUS_MAX = 5
	};

private:
	AntiDebug ad_{};

	//remote process variables
	std::vector<uint8_t> processName_{};
	uint16_t rProcessId_{};

	//control variable
	status stat_{ STATUS_IDLE };

	//Logging
	void WarningMsg(const std::string& msg);
	void ErrorMsg(const std::string& msg);

	//util
	bool Init(const std::string& processName);
	bool InitRemoteProcess(const std::string& processName);
	uint16_t GetProcessID(std::vector<uint8_t>& processName);

	//control
	void Play();
	void Pause();
	void End();

protected:
	//thread variables
	uint32_t workingTick_{ WORKER_TICK };
	std::thread workerThread_{};
	std::mutex dataMtx_{}, statusMtx_{};
	std::condition_variable cv_{};

	void WorkerFunc();
public:
	static constexpr uint32_t WORKER_TICK = 100;
	static constexpr uint32_t PROCESS_LIST_NUM = 1024;


	explicit AntiCheat(const std::string& processName);
	~AntiCheat();

	//control
	void Run();
	void Stop();
};