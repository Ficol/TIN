#ifndef BULLET_H
#define BULLET_H

#include "common.h"

#include <vector>
#include <cmath>

class Bullet
{
public:
    Bullet(char player_id_, game::Position board_size_, game::Position position_, size_t direction, size_t velocity_);

    bool update(double time);
    std::vector<char> getState() const;
    char getId() const;
    game::Position getPosition() const;

private:
    const static size_t POSSIBLE_DIRECTIONS_AMOUNT = 255;
    char player_id;
    game::Position board_size;
    game::Position position;
    size_t direction;
    size_t velocity;
};

#endif // BULLET_H