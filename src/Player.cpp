#include "Player.h"

Player::Player(char id_, game::Position position_, game::settings settings) : board_size(game::Position(settings.board_width, settings.board_height)),
                                                                        velocity(settings.bullet_velocity),  id(id_), position(position_),
                                                                        points(0)
{
}

void Player::resetPoints()
{
    points = 0;
}

void Player::respawn(game::Position position_)
{
    position = position_;
}

void Player::update(double time)
{
    auto now = std::chrono::steady_clock::now();
    switch (direction)
    {
    case game::STOP:
        break;
    case game::UP:
        position.second -= std::min(static_cast<size_t>(velocity * time), position.second);
        break;
    case game::UPRIGHT:
        position.first += std::min(static_cast<size_t>(velocity * time / sqrt(2)), board_size.first - position.first);
        position.second -= std::min(static_cast<size_t>(velocity * time / sqrt(2)), position.second);
        break;
    case game::RIGHT:
        position.first += std::min(static_cast<size_t>(velocity * time), board_size.first - position.first);
        break;
    case game::DOWNRIGHT:
        position.first += std::min(static_cast<size_t>(velocity * time / sqrt(2)), board_size.first - position.first);
        position.second += std::min(static_cast<size_t>(velocity * time / sqrt(2)), board_size.second - position.second);
        break;
    case game::DOWN:
        position.second += std::min(static_cast<size_t>(velocity * time), board_size.second - position.second);
        break;
    case game::DOWNLEFT:
        position.first -= std::min(static_cast<size_t>(velocity * time / sqrt(2)), position.first);
        position.second += std::min(static_cast<size_t>(velocity * time / sqrt(2)), board_size.second - position.second);
        break;
    case game::LEFT:
        position.first -= std::min(static_cast<size_t>(velocity * time), position.first);
        break;
    case game::UPLEFT:
        position.first -= std::min(static_cast<size_t>(velocity * time / sqrt(2)), position.first);
        position.second -= std::min(static_cast<size_t>(velocity * time / sqrt(2)), position.second);
        break;
    }
    if (std::chrono::duration<double>(now - last_move_update).count() > MOVE_TIME)
        direction = game::STOP;
}

std::vector<char> Player::getState() const
{
    std::vector<char> state = {
        id,
        static_cast<char>((position.first >> 8) & 0xff),
        static_cast<char>(position.first & 0xff),
        static_cast<char>((position.second >> 8) & 0xff),
        static_cast<char>(position.second & 0xff)};
    return state;
}

game::Position Player::getPosition() const
{
    return position;
}

size_t Player::getPoints() const
{
    return points;
}

char Player::getId() const
{
    return id;
}

void Player::move(game::move direction_)
{
    last_move_update = std::chrono::steady_clock::now();
    direction = direction_;
}

void Player::score()
{
    ++points;
}
