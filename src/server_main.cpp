#include <iostream>
#include "Server.h"

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }
    size_t port_number = atoi(argv[1]);
    try
    {
        Server game_server(port_number);
        game_server.run();
    }
    catch (std::runtime_error &e)
    {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
