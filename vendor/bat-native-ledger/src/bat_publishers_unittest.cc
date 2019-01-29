/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this file,
* You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/vendor/bat-native-ledger/include/bat/ledger/ledger.h"
#include "brave/vendor/bat-native-ledger/src/bat_publishers.h"
#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- brave_unit_tests --filter=BatPublishersTest.*

TEST_F(BatPublishersTest, calcScoreConsts) {
  BatPublishers publishers = new BatPublishers(nullptr);

  // Test 8 seconds
  publishers->calcScoreConsts(8);

  ASSERT_EQ(publishers.a_, 7000);
  ASSERT_EQ(publishers.a2_, 14000);
  ASSERT_EQ(publishers.a4_, 28000);
  ASSERT_EQ(publishers.b_, 1000);
  ASSERT_EQ(publishers.b2_, 1000000);
}