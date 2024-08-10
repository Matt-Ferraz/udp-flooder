#include <iostream>
#include <winsock2.h>
using namespace std;

#pragma comment(lib,"ws2_32.lib") 
#pragma warning(disable:4996) 

#define SERVER "185.41.143.41"  
#define BUFLEN 512  
#define PORT 9296  

class UDPClient {
public:
    UDPClient() {
        cout << "Initializing Winsock...\n";
        if (WSAStartup(MAKEWORD(2, 2), &ws) != 0) {
            cout << "Failed. Error Code: " << WSAGetLastError() << "\n";
            exit(EXIT_FAILURE);
        }
        cout << "Initialized.\n";

        if ((client_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR) {
            cout << "socket() failed with error code: " << WSAGetLastError() << "\n";
            exit(EXIT_FAILURE);
        }

        memset((char*)&server, 0, sizeof(server));
        server.sin_family = AF_INET;
        server.sin_port = htons(PORT);
        server.sin_addr.S_un.S_addr = inet_addr(SERVER);
    }

    ~UDPClient() {
        closesocket(client_socket);
        WSACleanup();
    }

    void start() {
        const char* message = "Hello from uly!";

        while (true) {
            if (sendto(client_socket, message, strlen(message), 0, (sockaddr*)&server, sizeof(sockaddr_in)) == SOCKET_ERROR) {
                cout << "sendto() failed with error code: " << WSAGetLastError() << "\n";
                exit(EXIT_FAILURE);
            }

            char answer[BUFLEN] = {};
            int slen = sizeof(sockaddr_in);

        }
    }

private:
    WSADATA ws;
    SOCKET client_socket;
    sockaddr_in server;
};

int main() {
    system("title UDP Client");

    MoveWindow(GetConsoleWindow(), 50, 50, 500, 500, true);

    UDPClient udpClient;
    udpClient.start();
}
