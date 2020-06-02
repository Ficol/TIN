#ifndef BULLET_H
#define BULLET_H

#include "common.h"

#include <vector>
#include <cmath>

class Bullet
{
public:
    Bullet(const char player_id_, const game::Position position_, const size_t direction);

    bool update(const double time);
    std::vector<char> getState() const;
    char getId() const;
    game::Position getPosition() const;
    void erase();
private:
    bool hit;
    const static size_t POSSIBLE_DIRECTIONS_AMOUNT = 255;
    char player_id;
    game::Position position;
    size_t direction;
};

#endif // BULLET_H