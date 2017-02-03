#include <boost/test/unit_test.hpp>
#include "communication/LcmHandler.h"

BOOST_AUTO_TEST_SUITE(LcmHandlerTestSuite)

BOOST_AUTO_TEST_CASE(PassTest) {
    LcmHandler handler;
    BOOST_CHECK_EQUAL(540, handler.calcBrakeAim(50, 10));
}

BOOST_AUTO_TEST_SUITE_END()
