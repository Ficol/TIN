#include "Server.h"

Server::Server(const size_t port_number)
{
    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port_number);
    server_address.sin_addr.s_addr = INADDR_ANY;

    if ((udp_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        throw std::runtime_error("UDP call error");
    if (bind(udp_socket, reinterpret_cast<sockaddr *>(&server_address), sizeof(server_address)) != 0)
        throw std::runtime_error("UDP bind call error");

    if ((listen_socket = socket(PF_INET, SOCK_STREAM, 0)) < 0)
        throw std::runtime_error("TCP call error");
    if (bind(listen_socket, reinterpret_cast<sockaddr *>(&server_address), sizeof(server_address)) != 0)
        throw std::runtime_error("TCP bind call error");
}

Server::~Server()
{
    close(listen_socket);
    close(udp_socket);
    for (auto &client : clients)
        close(client.getSocket());
}

void Server::run()
{
    if (listen(listen_socket, server::MAX_CLIENTS_AMOUNT) != 0)
        throw std::runtime_error("Listen call error");
    std::cout << "Listening..." << std::endl;
    std::thread udp_send(&Server::sendGameState, this, udp_socket);
    udp_send.detach();

    while (true)
    {
        sockaddr_in address;
        socklen_t address_size = sizeof(address);
        fd_set ready;
        FD_ZERO(&ready);
        FD_SET(listen_socket, &ready);
        FD_SET(udp_socket, &ready);
        int select_status = select(std::max(listen_socket, udp_socket) + 1, &ready, NULL, NULL, NULL);
        if (select_status < 0)
            throw std::runtime_error("Select call error");
        if (FD_ISSET(listen_socket, &ready))
        {
            int new_socket = accept(listen_socket, reinterpret_cast<sockaddr *>(&address), &address_size);
            if (new_socket < 0)
                throw std::runtime_error("Accept call error");
            if (clients.size() >= server::MAX_CLIENTS_AMOUNT)
                close(new_socket);
            else
            {
                std::thread new_tcp(&Server::handleConnection, this, new_socket);
                new_tcp.detach();
            }
        }
        if (FD_ISSET(udp_socket, &ready))
        {
            std::vector<char> receive_message(server::MAX_PACKET_SIZE);
            int nbytes = recvfrom(udp_socket, receive_message.data(), receive_message.size(), 0, reinterpret_cast<sockaddr *>(&address), &address_size);
            if (nbytes < 0)
                throw std::runtime_error("Recvfrom call error");
            if (nbytes < 2)
                continue;
            if (receive_message[0] == server::ID)
            {
                clients_mutex.lock();
                for (auto &client : clients)
                    if (receive_message[1] == client.getId() && !client.isInitialized())
                    {
                        client.initialize(address);
                        game_mutex.lock();
                        game.addPlayer(client.getId());
                        std::vector<char> init_message{server::INIT};
                        std::vector<char> settings = game.getSettings();
                        game_mutex.unlock();
                        init_message.insert(init_message.end(), settings.begin(), settings.end());
                        send(client.getSocket(), &init_message[0], init_message.size(), 0);
                        break;
                    }
                clients_mutex.unlock();
            }
            else if (receive_message[0] == server::PLAYER_MOVE || receive_message[0] == server::SHOOT)
            {
                clients_mutex.lock();
                for (auto &client : clients)
                    if (client.compareAddress(address))
                    {
                        std::vector<char> scorers;
                        game_mutex.lock();
                        char winner = game.update();
                        game.changeState(client.getId(), receive_message);
                        game_mutex.unlock();
                        if(winner)
                            sendTcpMessage(std::vector<char>{{server::TCP_GAME, winner}});
                        break;
                    }
                clients_mutex.unlock();
            }
        }
    }
}

void Server::handleConnection(int client_socket)
{
    //TODO entry configuration packet
    Client client(client_socket);
    client.setId();
    const std::vector<char> id_message{server::ID, client.getId()};
    send(client_socket, id_message.data(), id_message.size(), 0);
    std::cout << "Player " << static_cast<size_t>(static_cast<unsigned char>(client.getId())) << " connected" << std::endl;
    clients_mutex.lock();
    clients.push_back(client);
    clients_mutex.unlock();
    while (true)
    {
        timeval timeout = {server::MAX_DISCONNECTED_SECONDS, 0};
        fd_set ready;
        FD_ZERO(&ready);
        FD_SET(client_socket, &ready);
        int select_status = select(client_socket + 1, &ready, NULL, NULL, &timeout);
        if (select_status > 0 && FD_ISSET(client_socket, &ready))
        {
            char activity_message[server::MAX_PACKET_SIZE] = {0};
            int nbytes = recv(client_socket, activity_message, sizeof(activity_message), 0);
            if (nbytes <= 0)
                break;
        }
        else
            break;
    }
    game_mutex.lock();
    game.removePlayer(client.getId());
    game_mutex.unlock();
    clients_mutex.lock();
    closeConnection(client_socket);
    clients_mutex.unlock();
    std::cout << "Player " << static_cast<size_t>(static_cast<unsigned char>(client.getId())) << " disconnected" << std::endl;
}

void Server::sendGameState(int udp_socket)
{
    size_t packet_number = 0;
    while (true)
    {
        std::vector<char> scorers;
        game_mutex.lock();
        char winner = game.update();
        game_mutex.unlock();
        if(winner)
        {
            clients_mutex.lock();
            sendTcpMessage(std::vector<char>{server::TCP_GAME, winner});
            clients_mutex.unlock();
        }
        if (packet_number++ == 65535)
            packet_number = 0;
        std::vector<char> state_message{server::STATE};
        std::vector<char> packet_id = {
            static_cast<char>((packet_number >> 8) & 0xff),
            static_cast<char>(packet_number & 0xff)};
        game_mutex.lock();
        std::vector<char> state = game.getState();
        game_mutex.unlock();
        state_message.insert(state_message.end(), packet_id.begin(), packet_id.end());
        state_message.insert(state_message.end(), state.begin(), state.end());
        clients_mutex.lock();
        for (auto &client : clients)
            if (client.isInitialized())
            {
                sockaddr_in address = client.getAddr();
                sendto(udp_socket, state_message.data(), state_message.size(), 0, reinterpret_cast<sockaddr *>(&address), sizeof(address));
            }
        clients_mutex.unlock();
        std::this_thread::sleep_until(std::chrono::steady_clock::now() + std::chrono::milliseconds(server::STATE_PERIOD));
    }
}

void Server::sendTcpMessage(const std::vector<char> &message)
{
    for (auto &client : clients)
        if (client.isInitialized())
            send(client.getSocket(), message.data(), message.size(), 0);
}

void Server::closeConnection(const int socket)
{
    close(socket);
    clients.erase(std::remove_if(
                      clients.begin(), clients.end(),
                      [&](const Client &client) { return client.getSocket() == socket; }),
                  clients.end());
}