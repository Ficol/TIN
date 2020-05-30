#include "Game.h"

Game::Game() : last_update(std::chrono::steady_clock::now())
{
    std::srand(std::time(nullptr));
}

std::vector<char> Game::getSettings() const
{
    std::vector<char> settings_message{
        static_cast<char>((game::GAME_SETTINGS.board_width >> 8) & 0xff),
        static_cast<char>(game::GAME_SETTINGS.board_width & 0xff),
        static_cast<char>((game::GAME_SETTINGS.board_height >> 8) & 0xff),
        static_cast<char>(game::GAME_SETTINGS.board_height & 0xff),
        static_cast<char>(game::GAME_SETTINGS.win_score)};
    return settings_message;
}

std::vector<char> Game::getPoints() const
{
    std::vector<char> points_message;
    for (auto &player : players)
    {
        points_message.push_back(player.getId());
        size_t points = player.getPoints();
        points_message.push_back(static_cast<char>((points >> 8) & 0xff));
        points_message.push_back(static_cast<char>(points & 0xff));
    }
    return points_message;
}

void Game::reset()
{
    for (auto &player : players)
    {
        player.resetPoints();
        player.respawn(game::Position(rand() % game::GAME_SETTINGS.board_width, rand() % game::GAME_SETTINGS.board_height));
        bullets.clear();
    }
}

void Game::update(std::vector<char> &scorers)
{
    auto now = std::chrono::steady_clock::now();
    double time = std::chrono::duration<double>(now - last_update).count();
    last_update = now;
    for (auto &player : players)
        player.update(time);
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
                                 [&](Bullet &bullet) { return bullet.update(time); }),
                  bullets.end()); //update and erase if on border
    handleCollision(scorers);
}

void Game::changeState(const char id, const std::vector<char> &commands)
{
    if (commands.size() < 2)
        return;
    for (auto &player : players)
        if (player.getId() == id)
        {
            if (commands[0] == server::PLAYER_MOVE)
                player.move(static_cast<game::move>(commands[1]));
            else if (commands[0] == server::SHOOT && !player.isCooldown())
                shoot(player, static_cast<size_t>(commands[1]));
            else if (commands.size() >= 4 && commands[2] == server::SHOOT && !player.isCooldown())
                shoot(player, static_cast<size_t>(commands[3]));
        }
}

std::vector<char> Game::getState() const
{
    std::vector<char> state;
    for (auto &player : players)
    {
        std::vector<char> player_state = player.getState();
        state.insert(state.end(), player_state.begin(), player_state.end());
    }
    state.push_back(server::BULLET_POS);
    for (auto &bullet : bullets)
    {
        std::vector<char> bullets_state = bullet.getState();
        state.insert(state.end(), bullets_state.begin(), bullets_state.end());
    }
    return state;
}

void Game::addPlayer(const char id)
{
    players.push_back(Player(id, game::Position(rand() % game::GAME_SETTINGS.board_width, rand() % game::GAME_SETTINGS.board_height)));
}

void Game::removePlayer(const char id)
{
    players.erase(std::remove_if(
                      players.begin(), players.end(),
                      [&](const Player &player) { return player.getId() == id; }),
                  players.end());
}

void Game::shoot(Player &player, const size_t direction)
{
    player.shootUpdate();
    bullets.push_back(Bullet(player.getId(), player.getPosition(), direction));
}

void Game::handleCollision(std::vector<char> &scorers)
{
    for (auto &player : players)
        for (auto &bullet : bullets)
            if (player.getId() != bullet.getId())
                if (isShot(player.getPosition(), bullet.getPosition()))
                {
                    scorers.push_back(player.getId());
                    player.score();
                    break;
                }
}

bool Game::isShot(const game::Position player_position, const game::Position bullet_position) const
{
    size_t x_distance = static_cast<size_t>(std::abs(static_cast<int>(player_position.first) - static_cast<int>(bullet_position.first)));
    size_t y_distance = static_cast<size_t>(std::abs(static_cast<int>(player_position.second) - static_cast<int>(bullet_position.second)));
    size_t collision_distance = game::GAME_SETTINGS.player_size + game::GAME_SETTINGS.bullet_size;
    return x_distance * x_distance + y_distance * y_distance < collision_distance * collision_distance;
}