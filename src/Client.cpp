#include "Client.h"

size_t Client::count = 254;

Client::Client(int socket_) : socket(socket_), initialized(false)
{
}

void Client::setId()
{
    id = (++count % 255) + 1;
}

int Client::getSocket() const
{
    return socket;
}

char Client::getId() const
{

    return id;
}

sockaddr_in Client::getAddr() const
{
    return address;
}

bool Client::isInitialized() const
{
    return initialized;
}

void Client::initialize(const sockaddr_in address_)
{
    initialized = true;
    address = address_;
}

bool Client::compareAddress(const sockaddr_in &other) const
{
    if (address.sin_family == other.sin_family && address.sin_addr.s_addr == other.sin_addr.s_addr && address.sin_port == other.sin_port)
        return true;
    return false;
}