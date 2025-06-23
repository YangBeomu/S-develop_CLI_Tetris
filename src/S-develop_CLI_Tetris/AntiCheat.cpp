#include "pch.h"  
#include "AntiCheat.h"  

using namespace std;

AntiCheat::AntiCheat() {
	ad.Run();
	InitRemoteProcess();
}

AntiCheat::~AntiCheat() {
}

void AntiCheat::InitRemoteProcess() {
	vector<uint8_t> processName(sizeof(L"Memory_Detector.exe"));

	memcpy_s(processName.data(), processName.size(), L"Memory_Detector.exe", processName.size());
	if (!(rProcessId_ = GetProcessID(processName))) {
		STARTUPINFO si{};
		PROCESS_INFORMATION pi{};

		si.cb = sizeof(si);

		const wchar_t cmdLine[] = L"Memory_Detector.exe Tetris.exe monitor";

		if (CreateProcess(L"Memory_Detector.exe",
			(LPWSTR)cmdLine,
			NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi) == 0)
			throw runtime_error("Failed to create process");

		//rProcessHandle_ = pi.hProcess;

		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
}

uint16_t AntiCheat::GetProcessID(std::vector<uint8_t>& processName) {
	DWORD ret{};
	HANDLE handle{};

	if ((handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL)) == INVALID_HANDLE_VALUE)
		throw runtime_error("Failed to snapshot");

	PROCESSENTRY32 entry{};
	entry.dwSize = sizeof(entry);

	if (Process32First(handle, &entry)) {
		while (Process32Next(handle, &entry)) {
			if (memcmp(processName.data(), entry.szExeFile, processName.size()) == 0) {
				ret = entry.th32ProcessID;
				break;
			}
		}
	}

	return ret;
}