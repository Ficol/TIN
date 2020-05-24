#ifndef COMMON_H
#define COMMON_H

#include <cstddef>
#include <vector>

namespace server
{
    static const size_t MAX_PACKET_SIZE = 512;
    static const size_t MAX_CLIENTS_AMOUNT = 8;
    static const size_t MAX_DISCONNECTED_SECONDS = 10;
    static const size_t STATE_PERIOD = 33;
    static const char ID = 'X';
    static const char INIT = 'S';
    static const char STATE = 'Y';
    static const char BULLET_POS = 0;
    static const char TCP_GAME = 'E';
    static const char PLAYER_MOVE = 'T';
    static const char SHOOT = 'Z';
} // namespace server

namespace game
{
    struct settings
    {
        const size_t board_width = 60000;
        const size_t board_height = 60000;
        const size_t bullet_velocity = 20000;
        const size_t player_velocity = 10000;
        const unsigned char win_score = 12;
        const size_t player_size = 5000;
        const size_t bullet_size = 1000;
    } static const GAME_SETTINGS;
    enum move
    {
        STOP,
        UP,
        UPRIGHT,
        RIGHT,
        DOWNRIGHT,
        DOWN,
        DOWNLEFT,
        LEFT,
        UPLEFT
    };
} // namespace game

#endif //COMMON_H