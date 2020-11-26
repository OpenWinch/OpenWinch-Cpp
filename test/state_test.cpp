/**
 * @file state_test.cpp
 * @author Mickael GAILLARD (mick.gaillard@gmail.com)
 * @brief OpenWinch Project
 * 
 * @copyright Copyright © 2020
 */
#include "state.hpp"

#include "gtest/gtest.h"

class StateTest : public ::testing::Test {
 private:

 protected:

  StateTest() {
     // You can do set-up work for each test here.
  }

  ~StateTest() override {
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

TEST_F(StateTest, MethodCheckFault) {
  EXPECT_TRUE(State::checkFault(State::ERROR));
  EXPECT_FALSE(State::checkFault(State::UNKNOWN));
}
