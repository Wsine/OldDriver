#include <boost/test/unit_test.hpp>
#include "LcmHandler.h"

BOOST_AUTO_TEST_SUITE(LcmHandlerTestSuite)

BOOST_FIXTURE_TEST_CASE(PassTest, LcmHandler) {
    BOOST_CHECK_EQUAL(540, calcBrakeAim(50, 10));
}

BOOST_AUTO_TEST_SUITE_END()