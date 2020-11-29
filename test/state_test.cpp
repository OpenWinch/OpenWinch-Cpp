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

TEST_F(StateTest, MethodCheckRun) {
  EXPECT_TRUE(State::checkRun(State::RUNNING));
  EXPECT_TRUE(State::checkRun(State::START));
  EXPECT_FALSE(State::checkRun(State::UNKNOWN));
}

TEST_F(StateTest, MethodCheckStop) {
  EXPECT_TRUE(State::checkStop(State::STOP));
  EXPECT_TRUE(State::checkStop(State::IDLE));
  EXPECT_FALSE(State::checkStop(State::UNKNOWN));
}

TEST_F(StateTest, MethodCheckFault) {
  EXPECT_TRUE(State::checkFault(State::ERROR));
  EXPECT_FALSE(State::checkFault(State::UNKNOWN));
}

TEST_F(StateTest, MethodCheckInit) {
  EXPECT_TRUE(State::checkInit(State::INIT));
  EXPECT_FALSE(State::checkInit(State::UNKNOWN));
}

TEST_F(StateTest, MethodCheckBoot) {
  EXPECT_TRUE(State::checkBoot(State::INIT));
  EXPECT_FALSE(State::checkBoot(State::UNKNOWN));
}

// TEST_F(StateTest, MethodIsRun) {
//   EXPECT_TRUE(State::RUNNING.isRun());
//   EXPECT_TRUE(State::START.isRun());
//   EXPECT_FALSE(State::UNKNOWN.isRun());
// }

// TEST_F(StateTest, MethodIsStop) {
//   EXPECT_TRUE(State::STOP.isStop());
//   EXPECT_TRUE(State::IDLE.isStop());
//   EXPECT_FALSE(State::UNKNOWN.isStop());
// }

// TEST_F(StateTest, MethodIsFault) {
//   EXPECT_TRUE(State::ERROR.isFault());
//   EXPECT_FALSE(State::UNKNOWN.isFault());
// }

// TEST_F(StateTest, MethodIsInit) {
//   EXPECT_TRUE(State::INIT.isInit());
//   EXPECT_FALSE(State::UNKNOWN.isInit());
// }

// TEST_F(StateTest, MethodIsBoot) {
//   EXPECT_TRUE(State::UNKNOWN.isBoot());
//   EXPECT_FALSE(State::INIT.isBoot());
// }
