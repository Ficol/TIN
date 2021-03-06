#ifndef SERVER_H
#define SERVER_H

#include "common.h"

#include "Game.h"
#include "Client.h"
#include "Encryptor.h"

#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <algorithm>
#include <iostream>

#include <unistd.h>
#include <cstring>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

class Server
{
public:
    Server(const size_t port_number);
    ~Server();
    void run();

private:
    Game game;
    Encryptor encryptor;
    int listen_socket;
    int udp_socket;
    std::vector<Client> clients;
    std::mutex clients_mutex;
    std::mutex game_mutex;

    void handleConnection(const int client_socket);
    void sendGameState(const int udp_socket);
    void sendTcpMessage(const std::vector<char> &message);
    void closeConnection(const int socket);
};

#endif // SERVER_H