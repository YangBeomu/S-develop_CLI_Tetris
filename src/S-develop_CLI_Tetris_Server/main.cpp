#include "pch.h"
#include "S-develop_CLI_Tetris_Server.h"

using namespace std;

int main() {
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);

	NetworkManager nm(NetworkManager::SERVER, ntohl(Ip("0.0.0.0")), 8986);

	nm.Run();
	
	string input{};
	
	while (1) {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		int dataSize = nm.DataSize();
		if (dataSize != 0 && nm.DataSize() % 2 == 0) {
			auto first = nm.PopData();
			auto second = nm.PopData();

			

			cout << first.buffer_.data() << " : " 
				<< *(reinterpret_cast<float*>(second.buffer_.data())) << endl;
		}

		if (input == "exit") break;
	}
}