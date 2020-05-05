#ifndef SERVER_H
#define SERVER_H

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
#include <memory>

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
    static const int MAX_PACKET_SIZE = 512;
    static const int MAX_CLIENT_AMOUNT = 8;
    Game game;
    int listen_socket;
    int udp_socket;
    std::vector<std::unique_ptr<Client>> clients;
    std::mutex client_mutex;
    std::mutex game_mutex;

    void handleConnection(int client_socket);
    void sendGameState(int udp_socket);
    void closeConnection(int socket);
};

#endif // SERVER_H