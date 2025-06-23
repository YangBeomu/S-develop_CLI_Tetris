#include "pch.h"  
#include "AntiCheat.h"  

using namespace std;

AntiCheat::AntiCheat(const string& processName) {
	if (!Init(processName)) throw runtime_error("Failed to initialize AntiCheat");
}

AntiCheat::~AntiCheat() {
}

//logging
void AntiCheat::WarningMsg(const string& msg) {
	cerr << "[AntiCheat]" << msg << endl;
}

void AntiCheat::ErrorMsg(const string& msg) {
	cerr << "[AntiCheat]" << msg << endl;
	exit(1);
}


//util
bool AntiCheat::Init(const string& processName) {
	try {
		//ad_.Run();

		if (!InitRemoteProcess(processName)) throw runtime_error("Failed to call init remote process.");

		workerThread_ = std::thread(&AntiCheat::WorkerFunc, this);
	}
	catch (const exception& e) {
		WarningMsg(string("<Init> ").append(e.what()));
		return false;
	}
	return true;
}

bool AntiCheat::InitRemoteProcess(const string& processName) {
	try {
		if (processName.empty()) throw runtime_error("Process name cannot be empty");

		processName_.assign(processName.begin(), processName.end());

		//check loop execute
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));

		if (!(rProcessId_ = GetProcessID(processName_))) {
			STARTUPINFOA si{};
			PROCESS_INFORMATION pi{};

			si.cb = sizeof(si);

			string buffer(MAX_PATH, 0);

			GetModuleFileNameA(NULL, (LPSTR)buffer.data(), buffer.size());

			buffer = buffer.substr(buffer.find_last_of("\\/") + 1);

			string cmdLine(processName_.begin(), processName_.end());
			cmdLine.push_back(' ');
			cmdLine.append(buffer.data());
			cmdLine.append(" monitor");


			if (CreateProcessA(NULL,
				(LPSTR)cmdLine.c_str(),
				NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi) == 0)
				throw runtime_error("Failed to create process");

			std::this_thread::sleep_for(std::chrono::milliseconds(1000));

			//rProcessHandle_ = pi.hProcess;
			rProcessId_ = pi.dwProcessId;

			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
		}
	}
	catch (const exception& e) {
		WarningMsg(string("<InitRemoteProcess> ").append(e.what()));
		return false;
	}

	return true;
}

uint16_t AntiCheat::GetProcessID(std::vector<uint8_t>& processName) {
	DWORD ret{};
	HANDLE handle{};

	if ((handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL)) == INVALID_HANDLE_VALUE)
		throw runtime_error("Failed to snapshot");

	PROCESSENTRY32 entry{};
	entry.dwSize = sizeof(entry);

	string name(BUFSIZ*2, 0);

	if (Process32First(handle, &entry)) {
		while (Process32Next(handle, &entry)) {
			if (name.size() < sizeof(entry.szExeFile) * 2)
				name.resize(sizeof(entry.szExeFile) * 2);

			int len = WideCharToMultiByte(CP_ACP, 0, entry.szExeFile, -1, (LPSTR)name.c_str(), name.size(), NULL, NULL);

			if (memcmp(processName.data(), name.data(), processName.size()) == 0) {
				ret = entry.th32ProcessID;
				break;
			}
		}
	}

	return ret;
}

//control
void AntiCheat::Play() {
	cv_.notify_all();
}

void AntiCheat::Pause() {
	unique_lock<mutex> lck(statusMtx_);
	cv_.wait(lck);
}

void AntiCheat::End() {
	stat_ = STATUS_ENDED;
	Play();
}

void AntiCheat::Run() {
	stat_ = STATUS_RUNNING;

	Play();
}

void AntiCheat::Stop() {
	unique_lock<mutex> lck(dataMtx_);

	stat_ = STATUS_STOPPED;
}

void AntiCheat::WorkerFunc() {
	while (1) {
		try {
			std::this_thread::sleep_for(chrono::milliseconds(workingTick_));
			switch (stat_) {
			case STATUS_IDLE: {
				Pause();
				break;
			}
			case STATUS_RUNNING: {
				if(GetProcessID(processName_) != rProcessId_) exit(1);
				break;
			}
			case STATUS_STOPPED: {
				Pause();
				break;
			}
			case STATUS_ENDED: {
				return;
				break;
			}
			case STATUS_ERROR: {

				break;
			}
			default:
				break;
			}
		}
		catch (const exception& e) {
			ErrorMsg(string("<WorkerFunc> ").append(e.what()));
		}
	}
}

