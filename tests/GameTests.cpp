#include <boost/test/unit_test.hpp>

#include <common.h>

#include <Game.h>

BOOST_AUTO_TEST_SUITE(getSettings)

BOOST_AUTO_TEST_CASE(default_settings)
{
    Game game;
    std::vector<char> settings = game.getSettings();
    std::vector<char> expected{
        static_cast<char>(game::GAME_SETTINGS.win_score)};
    BOOST_CHECK_EQUAL_COLLECTIONS(settings.begin(), settings.end(),
                                  expected.begin(), expected.end());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(getState)

BOOST_AUTO_TEST_CASE(noPlayersNoBullets)
{
    Game game;
    std::vector<char> state = game.getState();
    std::vector<char> expected{0};
    BOOST_CHECK_EQUAL_COLLECTIONS(state.begin(), state.end(),
                                  expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE(onePlayerNoBullets)
{
    Game game;
    game.addPlayer(1);
    std::vector<char> state = game.getState();
    BOOST_CHECK_EQUAL(state[0], 1);
    BOOST_CHECK_EQUAL(state[1], 0);
    BOOST_CHECK_EQUAL(state[6], 0);
    BOOST_CHECK_EQUAL(state.size(), 7);
}

BOOST_AUTO_TEST_CASE(manyPlayersNoBullets)
{
    Game game;
    game.addPlayer(1);
    game.addPlayer(3);
    std::vector<char> state = game.getState();
    BOOST_CHECK_EQUAL(state[0], 1);
    BOOST_CHECK_EQUAL(state[1], 0);
    BOOST_CHECK_EQUAL(state[6], 3);
    BOOST_CHECK_EQUAL(state[7], 0);
    BOOST_CHECK_EQUAL(state[12], 0);
    BOOST_CHECK_EQUAL(state.size(), 13);
}

BOOST_AUTO_TEST_CASE(onePlayerOneBullet)
{
    Game game;
    game.addPlayer(5);
    std::vector<char> commands{'Z', 0};
    game.changeState(5, commands);
    std::vector<char> state = game.getState();
    BOOST_CHECK_EQUAL(state[0], 5);
    BOOST_CHECK_EQUAL(state[1], 0);
    BOOST_CHECK_EQUAL(state[6], 0);
    BOOST_CHECK_EQUAL(state.size(), 11);
}

BOOST_AUTO_TEST_CASE(onePlayerManyBullets)
{
    Game game;
    game.addPlayer(5);
    std::vector<char> commands{'Z', 0};
    game.changeState(5, commands);
    commands = {'Z', 100};
    game.changeState(5, commands);
    std::vector<char> state = game.getState();
    BOOST_CHECK_EQUAL(state[0], 5);
    BOOST_CHECK_EQUAL(state[1], 0);
    BOOST_CHECK_EQUAL(state[6], 0);
    BOOST_CHECK_EQUAL(state.size(), 15);
}

BOOST_AUTO_TEST_CASE(manyPlayersManyBullets)
{
    Game game;
    game.addPlayer(5);
    game.addPlayer(120);
    std::vector<char> commands{'Z', 0};
    game.changeState(5, commands);
    commands = {'Z', 100};
    game.changeState(5, commands);
    commands = {'Z', 10};
    game.changeState(120, commands);
    std::vector<char> state = game.getState();
    BOOST_CHECK_EQUAL(state[0], 5);
    BOOST_CHECK_EQUAL(state[1], 0);
    BOOST_CHECK_EQUAL(state[6], 120);
    BOOST_CHECK_EQUAL(state[7], 0);
    BOOST_CHECK_EQUAL(state[12], 0);
    BOOST_CHECK_EQUAL(state.size(), 25);
}

BOOST_AUTO_TEST_SUITE_END()