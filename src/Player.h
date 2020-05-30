#ifndef PLAYER_H
#define PLAYER_H

#include "common.h"

#include "Bullet.h"

#include <vector>
#include <chrono>

class Player
{
public:
    Player(const char id, const game::Position position_);

    void resetPoints();
    void respawn(const game::Position position_);
    void update(const double time);
    std::vector<char> getState() const;
    size_t getPoints() const;
    char getId() const;
    game::Position getPosition() const;
    void move(const game::move direction_);
    void score();
    void shootUpdate();
    bool isCooldown() const;

private:
    char id;
    game::Position position;
    game::move direction;
    size_t points;
    std::chrono::time_point<std::chrono::steady_clock> last_move_update;
    std::chrono::time_point<std::chrono::steady_clock> last_shoot;
};

#endif // PLAYER_H