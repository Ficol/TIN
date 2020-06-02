#include "Game.h"

Game::Game() : last_update(std::chrono::steady_clock::now())
{
    std::srand(std::time(nullptr));
}

std::vector<char> Game::getSettings() const
{
    std::vector<char> settings_message{
        static_cast<char>(game::GAME_SETTINGS.win_score)};
    return settings_message;
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

char Game::update()
{
    auto now = std::chrono::steady_clock::now();
    double time = std::chrono::duration<double>(now - last_update).count();
    last_update = now;
    for (auto &player : players)
        player.update(time);
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
                                 [&](Bullet &bullet) { return bullet.update(time); }),
                  bullets.end()); //update and erase if on border or hit
    handleCollision();
    for (auto &player : players)
        if (player.getPoints() >= game::GAME_SETTINGS.win_score)
            return player.getId();
    return 0;
}

void Game::changeState(const char id, const std::vector<char> &commands)
{
    for (auto &player : players)
        if (player.getId() == id)
        {
            if (commands[0] == server::PLAYER_MOVE)
                player.move(static_cast<game::move>(commands[1]));
            else if (commands[0] == server::SHOOT && !player.isCooldown())
                shoot(player, static_cast<size_t>(static_cast<unsigned char>(commands[1])));
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

void Game::handleCollision()
{
    for (auto &player : players)
        for (auto &bullet : bullets)
            if (player.getId() != bullet.getId())
                if (isShot(player.getPosition(), bullet.getPosition()))
                {
                    for(auto &player : players)
                        if(bullet.getId() == player.getId())
                            player.score();
                    bullet.erase();
                    player.respawn(game::Position(rand() % game::GAME_SETTINGS.board_width, rand() % game::GAME_SETTINGS.board_height));
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