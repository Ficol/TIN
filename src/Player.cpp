#include "Player.h"

Player::Player(char id_, Position position_, game::settings settings) : id(id_), position(position_), points(0),
                                                                        board_size(Position(settings.board_width, settings.board_height)),
                                                                        velocity(settings.bullet_velocity), bullets_velocity(settings.bullet_velocity),
                                                                        last_update(std::chrono::steady_clock::now())
{
}

void Player::resetPoints()
{
    points = 0;
}

void Player::respawn(Position position_)
{
    position = position_;
}

void Player::update()
{
    auto now = std::chrono::steady_clock::now();
    double time = std::chrono::duration<double>(now - last_update).count();
    last_update = now;
    switch (direction)
    {
    case game::STOP:
        break;
    case game::UP:
        position.second += std::min(static_cast<size_t>(velocity * time), board_size.second - position.second);
        break;
    case game::UPRIGHT:
        position.first += std::min(static_cast<size_t>(velocity * time / sqrt(2)), board_size.first - position.first);
        position.second += std::min(static_cast<size_t>(velocity * time / sqrt(2)), board_size.second - position.second);
        break;
    case game::RIGHT:
        position.first += std::min(static_cast<size_t>(velocity * time), board_size.first - position.first);
        break;
    case game::DOWNRIGHT:
        position.first += std::min(static_cast<size_t>(velocity * time / sqrt(2)), board_size.first - position.first);
        position.second -= std::min(static_cast<size_t>(velocity * time / sqrt(2)), board_size.second - position.second);
        break;
    case game::DOWN:
        position.second -= std::min(static_cast<size_t>(velocity * time), board_size.second - position.second);
        break;
    case game::DOWNLEFT:
        position.first -= std::min(static_cast<size_t>(velocity * time / sqrt(2)), board_size.first - position.first);
        position.second -= std::min(static_cast<size_t>(velocity * time / sqrt(2)), board_size.second - position.second);
        break;
    case game::LEFT:
        position.first -= std::min(static_cast<size_t>(velocity * time), board_size.first - position.first);
        break;
    case game::UPLEFT:
        position.first -= std::min(static_cast<size_t>(velocity * time / sqrt(2)), board_size.first - position.first);
        position.second += std::min(static_cast<size_t>(velocity * time / sqrt(2)), board_size.second - position.second);
        break;
    }
    if (std::chrono::duration<double>(now - last_move_update).count() > 0.2)
        direction = game::STOP;
    for (auto &bullet : bullets)
        bullet.update(time);
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

std::vector<char> Player::getBulletsState() const
{
    std::vector<char> bullets_state;
    for (auto &bullet : bullets)
    {
        std::vector<char> bullet_state = bullet.getState();
        bullets_state.insert(bullets_state.end(), bullet_state.begin(), bullet_state.end());
    }
    return bullets_state;
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

void Player::shoot(size_t direction_)
{
    bullets.push_back(Bullet(board_size, position, direction_, bullets_velocity));
}