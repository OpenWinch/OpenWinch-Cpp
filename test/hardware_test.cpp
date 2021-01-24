/**
 * @file hardware_test.cpp
 * @author Mickael GAILLARD (mick.gaillard@gmail.com)
 * @brief OpenWinch Project
 * 
 * @copyright Copyright © 2020-2021
 */
#include "mode.hpp"
#include "controller_mock.hpp"
#include "hardware_mock.hpp"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

class SpeedModeTest : public ::testing::Test {
 private:

 protected:
  SpeedModeTest() { }
  ~SpeedModeTest() override { }

  void SetUp() override { }
  void TearDown() override { }
};

TEST_F(SpeedModeTest, MethodToString) {
  EXPECT_STRCASEEQ("LOW", std::string(SpeedMode(SpeedMode::LOW)).c_str());
  EXPECT_STRCASEEQ("MEDIUM", std::string(SpeedMode(SpeedMode::MEDIUM)).c_str());
  EXPECT_STRCASEEQ("HIGH", std::string(SpeedMode(SpeedMode::HIGH)).c_str());
}

TEST_F(SpeedModeTest, MethodOperator) {
  EXPECT_TRUE(SpeedMode(SpeedMode::HIGH) == SpeedMode(SpeedMode::HIGH));
  EXPECT_FALSE(SpeedMode(SpeedMode::HIGH) == SpeedMode(SpeedMode::LOW));

  EXPECT_TRUE(SpeedMode(SpeedMode::HIGH) != SpeedMode(SpeedMode::LOW));
  EXPECT_FALSE(SpeedMode(SpeedMode::HIGH) != SpeedMode(SpeedMode::HIGH));
}

class BoardTest : public ::testing::Test {
 private:

 protected:

  BoardTest() {
     // You can do set-up work for each test here.
  }

  ~BoardTest() override {
     // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  void SetUp() override {
     // Code here will be called immediately after the constructor (right
     // before each test).
  }

  void TearDown() override {
     // Code here will be called immediately after each test (right
     // before the destructor).
  }

  // Class members declared here can be used by all tests in the test suite
  // for Foo.
};

TEST_F(BoardTest, MethodReverse) {
  EXPECT_TRUE(1);
}