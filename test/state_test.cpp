/**
 * @file state_test.cpp
 * @author Mickael GAILLARD (mick.gaillard@gmail.com)
 * @brief OpenWinch Project
 * 
 * @copyright Copyright © 2020-2021
 */
#include "state.hpp"

#include "gtest/gtest.h"

class StateTest : public ::testing::Test {
 private:

 protected:
  StateTest() { }
  ~StateTest() override { }

  void SetUp() override { }
  void TearDown() override { }
};

TEST_F(StateTest, MethodToString) {
  EXPECT_STRCASEEQ("UNKNOWN", std::string(State(State::UNKNOWN)).c_str());
  EXPECT_STRCASEEQ("HALT", std::string(State(State::HALT)).c_str());
  EXPECT_STRCASEEQ("BOOTED", std::string(State(State::BOOTED)).c_str());
  EXPECT_STRCASEEQ("ERROR", std::string(State(State::ERROR)).c_str());
  EXPECT_STRCASEEQ("INIT", std::string(State(State::INIT)).c_str());
  EXPECT_STRCASEEQ("IDLE", std::string(State(State::IDLE)).c_str());
  EXPECT_STRCASEEQ("START", std::string(State(State::START)).c_str());
  EXPECT_STRCASEEQ("RUNNING", std::string(State(State::RUNNING)).c_str());
  EXPECT_STRCASEEQ("STOP", std::string(State(State::STOP)).c_str());
}

TEST_F(StateTest, MethodOperator) {
  EXPECT_TRUE(State(State::UNKNOWN) == State(State::UNKNOWN));
  EXPECT_FALSE(State(State::UNKNOWN) == State(State::ERROR));

  EXPECT_TRUE(State(State::UNKNOWN) != State(State::ERROR));
  EXPECT_FALSE(State(State::UNKNOWN) != State(State::UNKNOWN));
}

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

TEST_F(StateTest, MethodIsRun) {
  EXPECT_TRUE(State(State::RUNNING).isRun());
  EXPECT_TRUE(State(State::START).isRun());
  EXPECT_FALSE(State(State::UNKNOWN).isRun());
}

TEST_F(StateTest, MethodIsStop) {
  EXPECT_TRUE(State(State::STOP).isStop());
  EXPECT_TRUE(State(State::IDLE).isStop());
  EXPECT_FALSE(State(State::UNKNOWN).isStop());
}

TEST_F(StateTest, MethodIsFault) {
  EXPECT_TRUE(State(State::ERROR).isFault());
  EXPECT_FALSE(State(State::UNKNOWN).isFault());
}

TEST_F(StateTest, MethodIsInit) {
  EXPECT_TRUE(State(State::INIT).isInit());
  EXPECT_FALSE(State(State::UNKNOWN).isInit());
}

TEST_F(StateTest, MethodIsBoot) {
  EXPECT_TRUE(State(State::INIT).isBoot());
  EXPECT_FALSE(State(State::UNKNOWN).isBoot());
}
