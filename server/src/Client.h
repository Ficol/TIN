#ifndef CLIENT_H
#define CLIENT_H

#include <sys/socket.h>

#include <string>
#include <random>
#include <ctime>

class Client
{
public:
    int socket;
    sockaddr_storage address;
    bool initialized;
    std::string unique_id;

    Client(int socket_);

private:
    std::string generateRandomString(std::size_t length = 5) const;
};

#endif // CLIENT_H