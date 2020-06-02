#ifndef COMMON_H
#define COMMON_H

#include <cstddef>
#include <utility>

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
    using Position = std::pair<size_t, size_t>;
    struct settings
    {
        size_t board_width = 60000;
        size_t board_height = 60000;
        size_t bullet_velocity = 40000;
        size_t player_velocity = 20000;
        unsigned char win_score = 12;
        size_t player_size = 3000;
        size_t bullet_size = 1000;
        double move_time = 0.2;
        double shoot_cooldown = 0.3;
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