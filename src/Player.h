#ifndef PLAYER_H
#define PLAYER_H

#include "common.h"

#include "Bullet.h"

#include <vector>
#include <random>
#include <chrono>

class Player
{
public:
    Player(char id, game::Position position_, game::settings settings);

    void resetPoints();
    void respawn(game::Position position_);
    void update(double time);
    std::vector<char> getState() const;
    size_t getPoints() const;
    char getId() const;
    game::Position getPosition() const;
    void move(game::move direction_);
    void score();

private:
    const static size_t MOVE_TIME = 0.2;
    game::Position board_size;
    size_t velocity;
    char id;
    game::Position position;
    game::move direction;
    size_t points;
    std::chrono::time_point<std::chrono::steady_clock> last_move_update;
};

#endif // PLAYER_H