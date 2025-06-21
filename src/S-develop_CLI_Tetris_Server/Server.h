#pragma once


class Server
{
private:
	SOCKET listenSocket_;
	std::vector<SOCKET> clientSockets_;
	bool isRunning_;
	int port_;
	std::string ipAddress_;

	std::thread listenThread_{};

	bool Initialize();
	void Cleanup();
	std::string GetLastErrorAsString() const;

protected:
	void ListenForClients();
	void HandleClient(SOCKET clientSocket);

public:
	Server(const std::string& ipAddress = "0.0.0.0", int port = 8080);
	~Server();

	// 서버 제어 메서드
	bool Start();
	void Stop();
	bool IsRunning() const { return isRunning_; }
};

