#include "Bullet.h"

Bullet::Bullet(char player_id_, game::Position board_size_, game::Position position_, size_t direction_, size_t velocity_) : player_id(player_id_), board_size(board_size_),
                                                                                                                 position(position_), direction(direction_), velocity(velocity_)
{
}

bool Bullet::update(double time)
{
    //TODO detect borders
    const double pi = std::acos(-1);
    double x = static_cast<double>(position.first) + velocity * time * std::cos(2 * pi * direction / POSSIBLE_DIRECTIONS_AMOUNT);
    double y = static_cast<double>(position.second) + velocity * time * std::sin(2 * pi * direction / POSSIBLE_DIRECTIONS_AMOUNT);
    if(x < 0 || x > board_size.first || y < 0 || y > board_size.second)
        return true;
    position.first = static_cast<size_t>(x);
    position.second = static_cast<size_t>(y);
    return false;
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

char Bullet::getId() const
{
    return player_id;
}

game::Position Bullet::getPosition() const
{
    return position;
}