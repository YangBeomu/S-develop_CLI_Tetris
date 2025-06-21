#include "pch.h"
#include "AntiDebug.h"

using namespace std;

AntiDebug::AntiDebug() {
	debugCheckWorker = std::thread(&AntiDebug::DebuggingCheckThread, this);
}

AntiDebug::~AntiDebug() {
	End();
	if (debugCheckWorker.joinable()) debugCheckWorker.join();
}

//debug
bool AntiDebug::NT_SUCESS(NTSTATUS status) {
	return status >= 0;
}

bool AntiDebug::GetDebuggingWIthPEB() {
	return IsDebuggerPresent();
}

bool AntiDebug::GetDebuggingWIthGS() {
	bool isDebugged = false;

	void* peb = (void*)__readgsqword(0x60);
	isDebugged = *((uint8_t*)peb + 2);

	return isDebugged;
}

bool AntiDebug::GetDebuggingWithCheckRemotePresent() {
	BOOL ret{};

	try {
		if (!CheckRemoteDebuggerPresent(GetCurrentProcess(), &ret))
			throw runtime_error("Failed to call CheckRemoteDebuggerPresent");
	}
	catch (const exception& e) {
		cerr << "<GetDebuggingWithCheckRemotePresent> " << e.what() << endl;
	}

		return ret;
}

bool AntiDebug::GetDebuggingWIthNtQueryInformation() {
	HMODULE ntdll = GetModuleHandle(L"ntdll.dll");
	pNtQueryInformationProcess proc = reinterpret_cast<pNtQueryInformationProcess>(GetProcAddress(ntdll, "NtQueryInformationProcess"));
	DWORD debugPort = 0;
	NTSTATUS status = proc(GetCurrentProcess(), ProcessDebugPort, &debugPort, sizeof(DWORD), nullptr);

	return NT_SUCCESS(status) && debugPort != 0;
}

//status 
void AntiDebug::Continue() {
	cv_.notify_all();
}

void AntiDebug::Pause() {
	unique_lock<mutex> lck(mtx_);
	cv_.wait(lck);
}

void AntiDebug::End() {
	status_ = STATUS_ENDED;
	Continue();
}

void AntiDebug:: Run() {
	status_ = STATUS_RUNNING;
	Continue();
}

void AntiDebug::Stop() {
	status_ = STATUS_STOPPED;
}

void AntiDebug::DebuggingCheckThread() {
	try {
	while(1) {
		switch (status_) {
			case STATUS_IDLE: {
				Pause();
			}
			break;
			
			case STATUS_RUNNING: {
				if (GetDebuggingWIthPEB()) {
					cout << "peb" << endl;
					exit(1);
				}
				if (GetDebuggingWIthNtQueryInformation()) {
					cout << "ntquery" << endl;
					exit(1);
				}
			}
			break;	
			case STATUS_STOPPED: {
				Pause();
			}
			break;
			
			case STATUS_ERROR: {
				// ���� �߻� �� ó��
				throw runtime_error("Cause error");
				return;
			}

			break;
		case STATUS_ENDED:
			return;

		default:
			// �� �� ���� ���� ó��
			throw runtime_error("Something wrong!");
			return;
			break;
		}
	}
	}
	catch (const exception& e) {
		cerr << "<DebuggingCheckThread> " << e.what() << endl;
	}
}