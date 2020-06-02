#include "Bullet.h"

Bullet::Bullet(const char player_id_, const game::Position position_, const size_t direction_) : hit(false), player_id(player_id_), position(position_), direction(direction_)
{
}

bool Bullet::update(const double time)
{
    if(hit)
        return hit;
    const double pi = std::acos(-1);
    double x = static_cast<double>(position.first) + game::GAME_SETTINGS.bullet_velocity * time * std::cos(2 * pi * direction / POSSIBLE_DIRECTIONS_AMOUNT);
    double y = static_cast<double>(position.second) + game::GAME_SETTINGS.bullet_velocity * time * std::sin(2 * pi * direction / POSSIBLE_DIRECTIONS_AMOUNT);
    if (x < 0 || x > game::GAME_SETTINGS.board_width || y < 0 || y > game::GAME_SETTINGS.board_height)
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

void Bullet::erase()
{
    hit = true;
}