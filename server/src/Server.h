#ifndef SERVER_H
#define SERVER_H

#include "common.h"

#include "Game.h"
#include "Client.h"

#include "boost/lexical_cast.hpp"

#include <vector>
#include <thread>
#include <chrono>
#include <mutex>
#include <algorithm>
#include <iostream>
#include <string>

#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

class Server
{
public:
    Server(size_t port_number);
    ~Server();
    void run();

private:
    Game game;
    int listen_socket;
    int udp_socket;
    std::vector<Client> clients;
    std::mutex client_mutex;
    std::mutex game_mutex;

    void handleConnection(const int client_socket);
    void sendGameState(const int udp_socket);
    void sendTcpMessage(const char *message) const;
    void closeConnection(const int socket);
};

#endif // SERVER_H