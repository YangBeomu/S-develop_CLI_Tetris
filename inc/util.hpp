#pragma once

namespace utils {
	using namespace std;

	tstring GetTmpPathWithMkDir(const tstring& folderName) {
		tstring logPath{};

		//init log file
		try {
			std::vector<TCHAR> buffer(MAX_PATH);

			int ret{};
			ret = GetTempPath2(buffer.size(), buffer.data());
			buffer[ret] = '\0';

			logPath.append(buffer.data());
			logPath.append(folderName + (TEXT("\\")));

			if (!core::CreateDirectory(logPath.c_str()))
				throw runtime_error("Failed to create directory");
		}
		catch (const exception& e) {
			cerr << "[GetTempPath] " << e.what() << endl;
		}

		return logPath;
	}

	std::vector<std::vector<TCHAR>> GetTempFiles(const tstring& folderName) {
		vector<vector<TCHAR>> ret{};
		HANDLE handle{}, fileHandle{};

		//read log file
		try {
			std::tstring logPath = GetTmpPathWithMkDir(folderName);
			if (logPath.empty()) throw runtime_error("Failed to get tmp path.");


			core::ST_FILE_FINDDATA fdt{};

			if ((handle = core::FindFirstFile((logPath + TEXT("*")).c_str(), &fdt)) == NULL)
				throw runtime_error("Failed to find first file.");

			do {
				if ((fileHandle = core::CreateFile((logPath + fdt.strFileName).c_str(), core::GENERIC_READ_, core::E_FILE_DISPOSITION::OPEN_EXISTING_, NULL)) == NULL)
					continue;


				vector<TCHAR> buffer{};

				QWORD fileSize = core::GetFileSize(fileHandle);
				buffer.resize(static_cast<size_t>(fileSize));

				DWORD readed{};

				if (!core::ReadFile(fileHandle, buffer.data(), buffer.size(), &readed)) {
					core::CloseFile(fileHandle);
					continue;
				}

				ret.push_back(buffer);

				core::CloseFile(fileHandle);
				fileHandle = NULL;

			} while (core::FindNextFile(handle, &fdt));
		}
		catch (const exception& e) {
			cerr << "[GetTempFiles] " << e.what() << endl;
		}

		if (fileHandle) CloseHandle(fileHandle);

		return ret;
	}

	bool SaveFile(const tstring& folderName, const tstring& title, vector<char>& buffer) {
		HANDLE fileHandle{};
		std::tstring logPath = GetTmpPathWithMkDir(folderName);

		try {
			if ((fileHandle = core::CreateFile((logPath + title).c_str(), core::GENERIC_WRITE_, core::CREATE_ALWAYS_, NULL)) == NULL)
				throw runtime_error("Failed to creat file");

			DWORD writed{};

			if (!core::WriteFile(fileHandle, buffer.data(), buffer.size(), &writed))
				throw runtime_error("Failed to write file.");
		}
		catch (const exception& e) {
			cerr << "[SaveFile] " << e.what() << endl;
			return false;
		}

		if (fileHandle) CloseHandle(fileHandle);

		return true;
	}

	void RemoveFIles(const tstring& folderName) {
		HANDLE handle{};

		try {
			std::tstring logPath = GetTmpPathWithMkDir(folderName);
			if (logPath.empty()) throw runtime_error("Failed to get tmp path.");
			core::ST_FILE_FINDDATA fdt{};
			if ((handle = core::FindFirstFile((logPath + TEXT("*")).c_str(), &fdt)) == NULL)
				throw runtime_error("Failed to find first file.");
			do {
				if (!core::DeleteFile((logPath + fdt.strFileName).c_str())) {
				}
			} while (FindNextFile(handle, &fdt));
		}
		catch (const exception& e) {
			cerr << "[RemoveFiles] " << e.what() << endl;
		}

	}

	uint64_t GetTimeStamp() {
		auto now = chrono::system_clock::now();
		uint64_t ret = chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

		return ret;
	}

};
