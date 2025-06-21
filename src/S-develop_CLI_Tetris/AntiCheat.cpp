#include "pch.h"  
#include "AntiCheat.h"  

using namespace std;

AntiCheat::AntiCheat() {
	if (!ci.Connect()) throw runtime_error("Failed to connect");
	ad.Run();

	vector<uint8_t> processName(sizeof(L"Memory_Detector"));

	memcpy_s(processName.data(), processName.size(), L"Memory_Detector", processName.size());
	GetProcessID(processName);
}

DWORD AntiCheat::GetProcessID(std::vector<uint8_t>& processName) {
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