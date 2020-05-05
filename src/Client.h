#ifndef CLIENT_H
#define CLIENT_H

#include <sys/socket.h>

#include <string>
#include <random>
#include <ctime>

class Client
{
public:
    const int socket;
    sockaddr_storage address;
    bool initialized;
    const std::string unique_id;

    Client(int socket_);
    void setAddress(sockaddr_storage address_);

private:
    std::string generateId(std::size_t length = 3);
};

#endif // CLIENT_H