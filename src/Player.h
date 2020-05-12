#ifndef PLAYER_H
#define PLAYER_H

#include "common.h"

#include "Bullet.h"

#include <utility>
#include <vector>
#include <random>
#include <chrono>

class Player
{
public:
    using Position = std::pair<size_t, size_t>;
    Player(char id, Position position_, game::settings settings);

    void resetPoints();
    void respawn(Position position_);
    void update();
    std::vector<char> getState() const;
    std::vector<char> getBulletsState() const;
    size_t getPoints() const;
    char getId() const;
    void move(game::move direction_);
    void shoot(size_t direction_);

private:
    char id;
    Position position;
    game::move direction;
    size_t points;
    Position board_size;
    size_t velocity;
    size_t bullets_velocity;
    std::vector<Bullet> bullets;
    std::chrono::time_point<std::chrono::steady_clock> last_update;
    std::chrono::time_point<std::chrono::steady_clock> last_move_update;
};

#endif // PLAYER_H