#include "Client.h"

Client::Client(int socket_) : socket(socket_), initialized(false), unique_id(generateId())
{
}

void Client::setAddress(sockaddr_storage address_)
{
    address = address_;
}

std::string Client::generateId(std::size_t length)
{
    static const std::string alphabet = "0123456789";
    static std::default_random_engine rng(std::time(nullptr));
    static std::uniform_int_distribution<std::size_t> distribution(0, alphabet.size() - 1);

    std::string str("X");
    while (str.size() < length)
        str += alphabet[distribution(rng)];
    return str;
}