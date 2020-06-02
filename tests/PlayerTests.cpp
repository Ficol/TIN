#include <boost/test/unit_test.hpp>

#include <common.h>

#include <Player.h>

#include <vector>

BOOST_AUTO_TEST_SUITE(resetPoints)

BOOST_AUTO_TEST_CASE(defaultReset)
{
    Player player(1, game::Position(0, 0));
    player.score();
    player.resetPoints();
    BOOST_CHECK_EQUAL(player.getPoints(), 0);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(updatePlayer)

BOOST_AUTO_TEST_CASE(withoutMove)
{
    Player player(1, game::Position(0, 0));
    player.update(1.0);
    BOOST_CHECK_EQUAL(player.getPosition().first, 0);
    BOOST_CHECK_EQUAL(player.getPosition().second, 0);
}

BOOST_AUTO_TEST_CASE(move)
{
    Player player(1, game::Position(0, 100));
    player.move(game::DOWN);
    player.update(1.0);
    BOOST_CHECK_EQUAL(player.getPosition().first, 0);
    BOOST_CHECK_EQUAL(player.getPosition().second, 100 + game::GAME_SETTINGS.player_velocity * 1.0);
}

BOOST_AUTO_TEST_CASE(onEdgeMove)
{
    Player player(1, game::Position(0, 100));
    player.move(game::LEFT);
    player.update(1.0);
    BOOST_CHECK_EQUAL(player.getPosition().first, 0);
    BOOST_CHECK_EQUAL(player.getPosition().second, 100);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(getPlayerState)

BOOST_AUTO_TEST_CASE(defaultGetState)
{
    Player player(5, game::Position(100, 200));
    player.score();
    std::vector<char> state = player.getState();
    std::vector<char> expected{
        5,
        1,
        static_cast<char>((100 >> 8) & 0xff),
        static_cast<char>(100 & 0xff),
        static_cast<char>((200 >> 8) & 0xff),
        static_cast<char>(200 & 0xff)};
    BOOST_CHECK_EQUAL_COLLECTIONS(state.begin(), state.end(),
                                  expected.begin(), expected.end());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(playerScore)

BOOST_AUTO_TEST_CASE(defaultScore)
{
    Player player(5, game::Position(100, 200));
    player.score();
    player.score();
    BOOST_CHECK_EQUAL(player.getPoints(), 2);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(isCooldown)

BOOST_AUTO_TEST_CASE(noCooldownAfterInitialization)
{
    Player player(5, game::Position(100, 200));
    BOOST_CHECK(!player.isCooldown());
}

BOOST_AUTO_TEST_CASE(cooldownAfterShoot)
{
    Player player(5, game::Position(100, 200));
    player.shootUpdate();
    BOOST_CHECK(player.isCooldown());
}

BOOST_AUTO_TEST_SUITE_END()