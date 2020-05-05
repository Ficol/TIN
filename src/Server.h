#ifndef SERVER_H
#define SERVER_H

#include "Game.h"

#include "boost/lexical_cast.hpp"

#include <vector>
#include <thread>
#include <chrono>
#include <mutex>
#include <algorithm>
#include <iostream>
#include <string>
#include <tuple>

#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

class Server
{
public:
    using Client = std::tuple<int, sockaddr_storage, bool>;
    Server(size_t port_number);
    void run();

private:
    static const int MAX_CLIENT_AMOUNT = 8;
    Game game;
    int listen_socket;
    int udp_socket;
    std::vector<Client> clients;

    void handleConnection(int client_socket);
    void sendGameState(int udp_socket);
    void closeConnection(int socket);
};

#endif // SERVER_H