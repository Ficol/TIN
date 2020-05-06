#include "Client.h"

Client::Client(int socket_) : socket(socket_), initialized(false), unique_id(generateRandomString())
{
}

std::string Client::generateRandomString(std::size_t length) const
{
    static const std::string alphabet = "0123456789"
                                        "!@#$%^&*"
                                        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                        "abcdefghijklmnopqrstuvwxyz";
    static std::default_random_engine rng(std::time(nullptr));
    static std::uniform_int_distribution<std::size_t> distribution(0, alphabet.size() - 1);

    std::string str;
    while (str.size() < length)
        str += alphabet[distribution(rng)];
    return str;
}