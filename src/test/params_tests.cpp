// Copyright (c) 2020 The BitcoinSubsidium Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"

#include "test/test_BitcoinSubsidium.h"

#include <boost/test/unit_test.hpp>

struct RegtestingSetup : public BasicTestingSetup
{
    RegtestingSetup() : BasicTestingSetup(CBaseChainParams::REGTEST)
    {}
};

BOOST_FIXTURE_TEST_SUITE(params_tests, RegtestingSetup)

    BOOST_AUTO_TEST_CASE(period_x16r_test)
    {
        BOOST_TEST_MESSAGE("Running Period x16r Test");

        BOOST_CHECK(IsPeriodX16R(Params().GetConsensus(), 0));
        BOOST_CHECK(!IsPeriodX16R(Params().GetConsensus(), 15));
    }

    BOOST_AUTO_TEST_CASE(period_maintance_test)
    {
        BOOST_TEST_MESSAGE("Running Period maintance Test");

        BOOST_CHECK(IsPeriodMaintence(Params().GetConsensus(), 10));
        BOOST_CHECK(!IsPeriodMaintence(Params().GetConsensus(), 25));
    }

    BOOST_AUTO_TEST_CASE(period_scrypt2_test)
    {
        BOOST_TEST_MESSAGE("Running Period scrypt^2 Test");

        BOOST_CHECK(IsPeriodScrypt2(Params().GetConsensus(), 20));
        BOOST_CHECK(IsPeriodScrypt2(Params().GetConsensus(), 45015));
        BOOST_CHECK(!IsPeriodScrypt2(Params().GetConsensus(), 19));
    }

BOOST_AUTO_TEST_SUITE_END()