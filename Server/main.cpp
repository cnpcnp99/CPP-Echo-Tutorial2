#include<iostream>
#include<string>
#include<winsock2.h>
using namespace std;
void ShowErrorMessage(string message)
{
	cout << "[오류발생]: " << message << '\n';
	system("pause");
	exit(1);
}

int main()
{
	WSADATA wsaData;
	SOCKET serverSocket, clientSocket;
	SOCKADDR_IN serverAddress, clientAddress;
	int serverPort = 9876;
	char received[256];

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) // Winsock을초기화합니다.
		ShowErrorMessage("WSAStartup()");
	serverSocket = socket(PF_INET, SOCK_STREAM, 0); // TCP 소켓을생성합니다.
	if (serverSocket == INVALID_SOCKET)
		ShowErrorMessage("socket()");
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); // 4바이트정수를네트워크바이트형식으로
	serverAddress.sin_port = htons(serverPort); // 2바이트정수네트워크바이트형식으로
	if (bind(serverSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
		ShowErrorMessage("bind()");
	cout << "[현재상태] bind()\n";
	if (listen(serverSocket, 5) == SOCKET_ERROR)
		ShowErrorMessage("listen()");
	cout << "[현재상태] listen()\n";
	int sizeClientAddress = sizeof(clientAddress);
	clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddress, &sizeClientAddress);
	cout << "[현재상태] accept()\n";
	if (clientSocket == INVALID_SOCKET)
		ShowErrorMessage("accept()");
	while (1) { // 클라이언트의메시지를받아서그대로다시전달합니다.
		int length = recv(clientSocket, received, sizeof(received), 0);
		received[length] = NULL;
		cout << "[클라이언트메시지]: " << received << '\n';
		cout << "[메시지전송]: " << received << '\n';
		if (strcmp(received, "[exit]") == 0) {
			send(clientSocket, received, sizeof(received) - 1, 0);
			cout << "[서버종료]\n";
			break;
		}
		send(clientSocket, received, sizeof(received) - 1, 0);
	}
	closesocket(clientSocket);
	closesocket(serverSocket);
	WSACleanup();
	system("pause");
	return 0;
}
