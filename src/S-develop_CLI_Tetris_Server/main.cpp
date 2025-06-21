#include "pch.h"
#include "S-develop_CLI_Tetris_Server.h"

using namespace std;

int main() {
	Server server("0.0.0.0", 8986);

	string input{};

	server.Start();
	
	while (1) {
		getline(std::cin, input);

		if (input == "exit") break;
	}
}