#include "Server.h"

Server::Server(size_t port_number) : game(0)
{
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port_number);
    server_address.sin_addr.s_addr = INADDR_ANY;

    if ((udp_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        throw std::runtime_error("UDP call error");
    if (bind(udp_socket, (struct sockaddr *)&server_address, sizeof(server_address)) != 0)
        throw std::runtime_error("UDP bind call error");

    if ((listen_socket = socket(PF_INET, SOCK_STREAM, 0)) < 0)
        throw std::runtime_error("TCP call error");
    if (bind(listen_socket, (struct sockaddr *)&server_address, sizeof(server_address)) != 0)
        throw std::runtime_error("TCP bind call error");
}

Server::~Server()
{
    close(listen_socket);
    close(udp_socket);
    for (auto &&client : clients)
        close(client->socket);
}

void Server::run()
{
    struct sockaddr_storage address;

    if (listen(listen_socket, MAX_CLIENT_AMOUNT) != 0)
        throw std::runtime_error("Listen call error");

    std::thread udp_send(&Server::sendGameState, this, udp_socket);
    udp_send.detach();

    while (true)
    {
        socklen_t address_size = sizeof(address);
        fd_set ready;
        FD_ZERO(&ready);
        FD_SET(listen_socket, &ready);
        FD_SET(udp_socket, &ready);
        int select_status = select(std::max(listen_socket, udp_socket) + 1, &ready, NULL, NULL, NULL);
        if (select_status < 0)
            throw std::runtime_error("Select call error");
        if (FD_ISSET(listen_socket, &ready)) //new connection
        {
            int new_socket = accept(listen_socket, (struct sockaddr *)&address, &address_size);
            if (new_socket < 0)
                throw std::runtime_error("Accept call error");
            if (clients.size() >= MAX_CLIENT_AMOUNT)
                close(new_socket);
            else
            {
                std::thread new_tcp(&Server::handleConnection, this, new_socket);
                new_tcp.detach();
            }
        }
        if (FD_ISSET(udp_socket, &ready)) //udp receive
        {
            char message[MAX_PACKET_SIZE] = {0};
            int nbytes = recvfrom(udp_socket, message, sizeof(message), 0, (struct sockaddr *)&address, &address_size);
            if (nbytes < 0)
                throw std::runtime_error("Recvfrom call error");
            if (nbytes == 0)
                continue;
            std::string processed(message);
            if (processed.rfind("X", 0) == 0) //if initialization
            {
                client_mutex.lock();
                for (auto &&client : clients)
                    if ("X" + client->unique_id == processed)
                    {
                        char confirm[] = "start";
                        client->address = address;
                        client->initialized = true;
                        send(client->socket, confirm, strlen(confirm), 0);
                        break;
                    }
                client_mutex.unlock();
            }
            else //TODO if game move
            {
                int number = boost::lexical_cast<int>(processed);
                game_mutex.lock();
                game.changeState(number);
                game_mutex.unlock();
            }
        }
    }
}

void Server::handleConnection(int client_socket) //tcp connection
{
    //TODO entry configuration packet
    std::unique_ptr<Client> client = std::make_unique<Client>(Client(client_socket));
    const char *id = ("X" + client->unique_id).c_str();
    send(client_socket, id, strlen(id), 0);
    client_mutex.lock();
    clients.push_back(std::move(client));
    std::cout << "Socket " << client_socket << " connected" << std::endl;
    client_mutex.unlock();
    while (true)
    {
        struct timeval timeout = {10, 0};
        fd_set ready;
        FD_ZERO(&ready);
        FD_SET(client_socket, &ready);
        int select_status = select(client_socket + 1, &ready, NULL, NULL, &timeout);
        if (select_status < 0)
            throw std::runtime_error("Select call error");
        if (select_status > 0 && FD_ISSET(client_socket, &ready))
        {
            char message[MAX_PACKET_SIZE] = {0};
            int nbytes = recv(client_socket, message, sizeof(message), 0);
            if (nbytes <= 0)
                break;
        }
        else
            break;
    }
    client_mutex.lock();
    closeConnection(client_socket);
    std::cout << "Socket " << client_socket << " disconnected" << std::endl;
    client_mutex.unlock();
}

void Server::sendGameState(int udp_socket) //udp send
{
    char message[MAX_PACKET_SIZE];
    while (true)
    {
        game_mutex.lock();
        //TODO calculate game state
        int state = game.getState();
        if (game.getState() > 1000)
            game.reset();
        game_mutex.unlock();
        if (state > 1000)
            sendCommunicate((char*)"stop");
        strcpy(message, (boost::lexical_cast<std::string>(state)).c_str());
        client_mutex.lock();
        for (auto &&client : clients)
        {
            if (client->initialized == false)
                continue;
            sendto(udp_socket, message, strlen(message), 0, (struct sockaddr *)&client->address, sizeof(client->address));
        }
        client_mutex.unlock();
        std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::milliseconds(100));
    }
}

void Server::closeConnection(int socket)
{
    close(socket);
    clients.erase(std::remove_if(
                      clients.begin(), clients.end(),
                      [&](const std::unique_ptr<Client> &x) { return x->socket == socket; }),
                  clients.end());
}

void Server::sendCommunicate(char *message)
{
    for (auto &&client : clients)
        send(client->socket, message, strlen(message), 0);
}