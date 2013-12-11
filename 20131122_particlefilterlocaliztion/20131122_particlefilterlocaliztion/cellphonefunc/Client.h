#ifndef CLIENT_H
#define CLIENT_H

#include <WinSock2.h>

class Client {
public:
    Client(char* servername);
    bool Start();
    void Stop();
    bool Send(char* szMsg);
    bool Recv();

private:
    char* szServerName;
    SOCKET ConnectSocket;
};

#endif