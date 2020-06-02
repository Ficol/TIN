#include <boost/test/unit_test.hpp>

#include <common.h>

#include <Bullet.h>

#include <vector>

BOOST_AUTO_TEST_SUITE(bulletUpdate)

BOOST_AUTO_TEST_CASE(move)
{
    Bullet bullet(1, game::Position(0, 0), 0);
    BOOST_CHECK(!bullet.update(1.0));
    BOOST_CHECK_EQUAL(bullet.getPosition().first, game::GAME_SETTINGS.bullet_velocity * 1.0);
    BOOST_CHECK_EQUAL(bullet.getPosition().second, 0);
}

BOOST_AUTO_TEST_CASE(onEdgeMove)
{
    Bullet Bullet(1, game::Position(0, 100), 100);
    BOOST_CHECK(Bullet.update(10.0));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(getState)

BOOST_AUTO_TEST_CASE(defaultGetState)
{
    Bullet bullet(5, game::Position(100, 200), 100);
    std::vector<char> state = bullet.getState();
    std::vector<char> expected{
        static_cast<char>((100 >> 8) & 0xff),
        static_cast<char>(100 & 0xff),
        static_cast<char>((200 >> 8) & 0xff),
        static_cast<char>(200 & 0xff)};
    BOOST_CHECK_EQUAL_COLLECTIONS(state.begin(), state.end(),
                                  expected.begin(), expected.end());
}

BOOST_AUTO_TEST_SUITE_END()
