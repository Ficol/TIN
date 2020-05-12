#ifndef CLIENT_H
#define CLIENT_H

#include <netdb.h>
#include <cstring>

class Client
{
public:
    Client(const int socket_);

    void setId();
    int getSocket() const;
    char getId() const;
    sockaddr_in getAddr() const;
    bool isInitialized() const;
    void initialize(sockaddr_in address_);
    bool compareAddress(const sockaddr_in &other) const;

private:
    static size_t count;
    int socket;
    char id;
    sockaddr_in address;
    bool initialized;
};

#endif // CLIENT_H