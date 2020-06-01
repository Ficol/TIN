#include "Player.h"

Player::Player(const char id_, const game::Position position_) : id(id_), position(position_), points(0),
                                                     last_move_update(std::chrono::steady_clock::now()), last_shoot(std::chrono::steady_clock::now() - std::chrono::seconds{10})
{
}

void Player::resetPoints()
{
    points = 0;
}

void Player::respawn(const game::Position position_)
{
    position = position_;
}

void Player::update(const double time)
{
    auto now = std::chrono::steady_clock::now();
    switch (direction)
    {
    case game::STOP:
        break;
    case game::UP:
        position.second -= std::min(static_cast<size_t>(game::GAME_SETTINGS.player_velocity * time), position.second);
        break;
    case game::UPRIGHT:
        position.first += std::min(static_cast<size_t>(game::GAME_SETTINGS.player_velocity * time / sqrt(2)), game::GAME_SETTINGS.board_width - position.first);
        position.second -= std::min(static_cast<size_t>(game::GAME_SETTINGS.player_velocity * time / sqrt(2)), position.second);
        break;
    case game::RIGHT:
        position.first += std::min(static_cast<size_t>(game::GAME_SETTINGS.player_velocity * time), game::GAME_SETTINGS.board_width - position.first);
        break;
    case game::DOWNRIGHT:
        position.first += std::min(static_cast<size_t>(game::GAME_SETTINGS.player_velocity * time / sqrt(2)), game::GAME_SETTINGS.board_width - position.first);
        position.second += std::min(static_cast<size_t>(game::GAME_SETTINGS.player_velocity * time / sqrt(2)), game::GAME_SETTINGS.board_height - position.second);
        break;
    case game::DOWN:
        position.second += std::min(static_cast<size_t>(game::GAME_SETTINGS.player_velocity * time), game::GAME_SETTINGS.board_height - position.second);
        break;
    case game::DOWNLEFT:
        position.first -= std::min(static_cast<size_t>(game::GAME_SETTINGS.player_velocity * time / sqrt(2)), position.first);
        position.second += std::min(static_cast<size_t>(game::GAME_SETTINGS.player_velocity * time / sqrt(2)), game::GAME_SETTINGS.board_height - position.second);
        break;
    case game::LEFT:
        position.first -= std::min(static_cast<size_t>(game::GAME_SETTINGS.player_velocity * time), position.first);
        break;
    case game::UPLEFT:
        position.first -= std::min(static_cast<size_t>(game::GAME_SETTINGS.player_velocity * time / sqrt(2)), position.first);
        position.second -= std::min(static_cast<size_t>(game::GAME_SETTINGS.player_velocity * time / sqrt(2)), position.second);
        break;
    }
    if (std::chrono::duration<double>(now - last_move_update).count() > game::GAME_SETTINGS.move_time)
        direction = game::STOP;
}

std::vector<char> Player::getState() const
{
    std::vector<char> state = {
        id,
        points,
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

char Player::getPoints() const
{
    return points;
}

char Player::getId() const
{
    return id;
}

void Player::move(const game::move direction_)
{
    last_move_update = std::chrono::steady_clock::now();
    direction = direction_;
}

void Player::score()
{
    ++points;
}

void Player::shootUpdate()
{
    last_shoot = std::chrono::steady_clock::now();
}

bool Player::isCooldown() const
{
    if (std::chrono::duration<double>(std::chrono::steady_clock::now() - last_shoot).count() > game::GAME_SETTINGS.shoot_cooldown)
        return false;
    return true;
}