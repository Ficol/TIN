#include "Game.h"

Game::Game(game::settings settings_) : settings(settings_)
{
    std::srand(std::time(nullptr));
}

std::vector<char> Game::getSettings() const
{
    std::vector<char> settings_message{
        static_cast<char>((settings.board_width >> 8) & 0xff),
        static_cast<char>(settings.board_width & 0xff),
        static_cast<char>((settings.board_height >> 8) & 0xff),
        static_cast<char>(settings.board_height & 0xff),
        static_cast<char>(settings.win_score)};
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
        player.respawn(Position(rand() % settings.board_width, rand() % settings.board_height));
    }
}

char Game::update()
{
    for (auto &player : players)
        player.update();
    char scorer = checkCollision();
    if (scorer)
        for (auto &player : players)
            if (static_cast<int>(player.getPoints()) >= settings.win_score)
                reset();
    return scorer;
}

void Game::changeState(char id, const std::vector<char> &commands)
{
    if (commands.size() < 2)
        return;
    for (auto &player : players)
        if (player.getId() == id)
        {
            if (commands[0] == server::PLAYER_MOVE)
                player.move(static_cast<game::move>(commands[1]));
            if (commands[0] == server::SHOOT)
                player.shoot(static_cast<size_t>(commands[1]));
            else if (commands.size() >= 4 && commands[2] == server::SHOOT)
                player.shoot(static_cast<size_t>(commands[3]));
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
    for (auto &player : players)
    {
        std::vector<char> bullets_state = player.getBulletsState();
        state.insert(state.end(), bullets_state.begin(), bullets_state.end());
    }
    return state;
}

void Game::addPlayer(char id)
{
    players.push_back(Player(id, Position(rand() % settings.board_width, rand() % settings.board_height), settings));
}

void Game::removePlayer(char id)
{
    players.erase(std::remove_if(
                      players.begin(), players.end(),
                      [&](const Player &x) { return x.getId() == id; }),
                  players.end());
}

char Game::checkCollision()
{
    //if bullet on border or hit - erase
    //returns player id who got shot
    return 0;
}