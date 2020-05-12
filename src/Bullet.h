#ifndef BULLET_H
#define BULLET_H

#include "common.h"

#include <utility>
#include <vector>
#include <cmath>

class Bullet
{
public:
    using Position = std::pair<size_t, size_t>;
    Bullet(Position board_size_, Position position_, size_t direction, size_t velocity_);

    void update(double time);
    std::vector<char> getState() const;

private:
    const size_t POSSIBLE_DIRECTIONS_AMOUNT = 255;
    Position board_size;
    Position position;
    size_t direction;
    size_t velocity;
};

#endif // BULLET_H