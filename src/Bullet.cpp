#include "Bullet.h"

Bullet::Bullet(Position board_size_, Position position_, size_t direction_, size_t velocity_) : board_size(board_size_), position(position_),
                                                                                                direction(direction_), velocity(velocity_)
{
}

void Bullet::update(double time)
{
    const double pi = std::acos(-1);
    position.first += std::min(static_cast<size_t>(velocity * time * std::cos(2 * pi * direction / POSSIBLE_DIRECTIONS_AMOUNT)),
                               board_size.first - position.first);
    position.second += std::min(static_cast<size_t>(velocity * time * std::cos(2 * pi * direction / POSSIBLE_DIRECTIONS_AMOUNT)),
                                board_size.second - position.second);
}

std::vector<char> Bullet::getState() const
{
    std::vector<char> state = {
        static_cast<char>((position.first >> 8) & 0xff),
        static_cast<char>(position.first & 0xff),
        static_cast<char>((position.second >> 8) & 0xff),
        static_cast<char>(position.second & 0xff)};
    return state;
}