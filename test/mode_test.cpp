/**
 * @file mode_test.cpp
 * @author Mickael GAILLARD (mick.gaillard@gmail.com)
 * @brief OpenWinch Project
 * 
 * @copyright Copyright © 2020
 */
#include "mode.hpp"
#include "controller_mock.h"
#include "hardware_mock.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

class OneWayModeTest : public ::testing::Test {
 private:

 protected:

  OneWayModeTest() {
     // You can do set-up work for each test here.
  }

  ~OneWayModeTest() override {
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

TEST_F(OneWayModeTest, MethodExtraMode) {
  EXPECT_TRUE(1);
}


class TwoWayModeTest : public ::testing::Test {
 private:

 protected:

  TwoWayModeTest() {
     // You can do set-up work for each test here.
  }

  ~TwoWayModeTest() override {
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

TEST_F(TwoWayModeTest, MethodExtraMode) {
  EXPECT_TRUE(1);
}

TEST_F(TwoWayModeTest, MethodIsEndSecurity) {
  EXPECT_TRUE(1); // .isEndSecurity()
}


class InfinityModeTest : public ::testing::Test {
 private:
  ModeEngine* entity;

 protected:

  InfinityModeTest() {
    // You can do set-up work for each test here.
  }

  ~InfinityModeTest() override {
    // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  void SetUp() override {
    // Winch* winch = new MockWinch();
    // Board* board = new MockBoard(winch);

    // this->entity = new InfinityMode(board, winch);
  }

  void TearDown() override {
    delete this->entity;
    this->entity = 0;
  }

  // Class members declared here can be used by all tests in the test suite
  // for Foo.
};

TEST_F(InfinityModeTest, MethodExtraMode) {
  EXPECT_TRUE(1);
}
