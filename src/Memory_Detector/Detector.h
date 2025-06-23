#pragma once

class Detector
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

	enum Section {
		TEXT = 0,     // ���� �ڵ尡 ���Ե� ����
		RDATA = 1,    // �б� ���� ������ ����
		DATA = 2,     // �ʱ�ȭ�� ������ ����
		BSS = 3,      // �ʱ�ȭ���� ���� ������ ����
		IDATA = 4,    // ������ �Լ� ����(Import) ����
		EDATA = 5,    // ������ �Լ� ����(Export) ����
		PDATA = 6,    // ���� ó�� ���� ����
		RSRC = 7,     // ���ҽ� ����
		RELOC = 8,    // ���ġ ���� ����
		TLS = 9,      // ������ ���� ���丮�� ����
		DEBUG = 10,    // ����� ���� ����
		SECTION_COUNT
	};

	static constexpr const char* sectionList_[] = {
		(".text"),   // TEXT
		(".rdata"),  // RDATA
		(".data"),   // DATA
		(".bss"),    // BSS
		(".idata"),  // IDATA
		(".edata"),  // EDATA
		(".pdata"),  // PDATA
		(".rsrc"),   // RSRC
		(".reloc"),  // RELOC
		(".tls"),    // TLS
		(".debug"),  // DEBUG
	};

	static constexpr const TCHAR* exceptionDllList_[] = {
		TEXT("ntdll.dll"),
		TEXT("KERNEL32.DLL"),
		TEXT("KERNELBASE.dll"),
		TEXT("USER32.dll"),
		TEXT("GDI32.dll"),
		TEXT("gdi32full.dll"),
		TEXT("win32u.dll"),
		TEXT("ucrtbase.dll"),
		TEXT("msvcrt.dll"),
		TEXT("msvcp_win.dll"),
		TEXT("WS2_32.dll"),
		TEXT("RPCRT4.dll"),
		TEXT("IMM32.dll"),
		TEXT("kernel.appcore.dll"),
		TEXT("sechost.dll"),
		TEXT("advapi32.dll"),
		TEXT("bcryptPrimitives.dll"),
		TEXT("mswsock.dll"),
		TEXT("TextShaping.dll"),
		TEXT("combase.dll"),
		TEXT("OLEAUT32.dll"),
		TEXT("ole32.dll"),
		TEXT("uxtheme.dll"),
		TEXT("MSCTF.dll"),
		TEXT("textinputframework.dll"),
		TEXT("CoreMessaging.dll"),
		TEXT("CoreUIComponents.dll"),
		TEXT("wintypes.dll"),
		TEXT("CRYPTBASE.dll"),
		TEXT("SHCore.dll"),
		TEXT("SHLWAPI.dll"),
		TEXT("version.dll"),
		TEXT("cfgmgr32.dll"),
		TEXT("powrprof.dll"),
		TEXT("profapi.dll"),
	};

	struct LogInformation {
		uint64_t startTimestamp_{};
		uint64_t currentTimestamp_{};

		std::tstring processName_{};
		std::tstring processBinaryPath_{};
		std::vector<BYTE> processBinaryHash_{};

		std::map<Section, std::vector<BYTE>> sectionHashs_{};
		std::map<Section, std::vector<BYTE>> processSectionHashs_{};

		std::vector<std::tstring> dllList_{};
		std::vector<std::tstring> processDllList_{};

		std::map <std::string, std::vector<std::string>> iatInfo_{};
		std::map <std::string, std::vector<std::string>> processIatInfo_{};

		bool processRunning{};

		bool isEmpty() const {
			LogInformation li{};

			if (memcmp(this, &li, sizeof(LogInformation)) == 0)
				return true;

			return false;
		}
	};


private:
	struct ProcessInformation {
		HANDLE processHandle_{};
		uint16_t processId_{};

		bool empty() const {
			return processHandle_ == nullptr || processId_ == 0;
		}
	};

	using ProcessInformationPtr = ProcessInformation*;

	//static constexpr int READ_PROCESS_MEMORY_SIZE = 0x4000;
	static constexpr int MODS_COUNT = 0x400;

	//log variable
	uint64_t startTimeStamp_{};
	std::list<LogInformation> logList_{};

	//process variable
	std::tstring processName_{};
	std::tstring processBinaryPath_{};

	ProcessInformation pi_{};
	//sha256
	std::vector<BYTE> processBinaryHash_{}; 

	//.code section variable
	//std::vector<BYTE> textSectionBinary_{};
	std::map<Section, std::vector<BYTE>> sectionHashs_{};
	std::map<Section, std::vector<BYTE>> processSectionHashs_{};

	//dll variable
	std::vector<std::tstring> dllList_{};
	std::vector<std::tstring> processDllList_{};
	//std::vector<std::tstring> otherDllList_{};

	//iat variable
	std::map <std::string, std::vector<std::string>> iatInfo_{}; //dll, functions
	std::map <std::string, std::vector<std::string>> processIatInfo_{};
	//std::map <std::string, std::vector<std::string>> otherIatInfo_{};
	
	std::set<std::tstring> EATInfo_{};

	//Utils
	std::vector<BYTE> ReadBinary();
	HANDLE CreateDummyProcess();
	void DestroyDummyProcess(HANDLE handle);

	//LOG
	uint64_t GetTimeStamp();
	LogInformation GetLogInfo();

	//PROCESS
	ProcessInformation GetProcessInfromation();
	std::tstring GetProcessBinaryPath();
	uint16_t GetProcessID(const std::tstring& processName);

	bool IsProcessRunning(const std::tstring& processName);

	//HASH
	std::vector<BYTE> CalcHash(PBYTE buffer,uint bufferSize);

	//SECTION
	std::vector<Section> GetSectionTypes(std::vector<BYTE>& binary);
	std::vector<BYTE> GetSectionData(HANDLE handle, const Section& sc);
	std::map <Detector::Section, std::vector<BYTE>> GetAllSectionData(HANDLE handle);

	bool CompareSection(const Section sc);

	//DLL
	std::vector<std::tstring> GetDllList(HANDLE handle);
	
	//IAT
	DWORD CalcRVA(const IMAGE_NT_HEADERS* ntHeader, const DWORD& RVA);

	std::map<std::string, std::vector<std::string>> GetIAT(PBYTE dataPtr,bool dynamic = false); //iatInfo_<dllName, functionNames>
	std::set<std::tstring> GetEAT(core::CPEParser& parser);

	//Logging
	void WarningMsg(const std::string& msg);
	void ErrorMsg(const std::string& msg);

	//control
	Detector::status stat_{ STATUS_IDLE };

	void Play();
	void Pause();
	void End();

protected:
	//thread relative variables
	uint32_t workingTick_{};
	std::thread workerThread_{};
	std::mutex dataMtx_{}, statusMtx_{};
	std::condition_variable cv_{};

	void WorkerFunc();

public:
	explicit Detector(const std::tstring process, const uint32_t tick=1000);
	~Detector();
	//STAT
	void Run();
	void Stop();

	bool Detected();

	//HASH
	void PrintHash();
	bool CompareHash(const std::vector<BYTE>& hash);
	
	//DLL
	void PrintDLLs();
	bool CompareDLLs();
	
	//IAT
	void PrintIAT();
	bool CompareIAT();

	//LOG
	LogInformation GetLog();

	bool detect(std::vector<BYTE>& hash);
};

