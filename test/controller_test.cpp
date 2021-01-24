// /**
//  * @file controller_test.cpp
//  * @author Mickael GAILLARD (mick.gaillard@gmail.com)
//  * @brief OpenWinch Project
//  * 
//  * @copyright Copyright © 2020-2021
//  */


// #include "gtest/gtest.h"

// class WinchTest : public ::testing::Test {
//  private:
//   TestWinch* w = nullptr;

//  protected:

//   WinchTest() {
//      // You can do set-up work for each test here.
//   }

//   ~WinchTest() override {
//      // You can do clean-up work that doesn't throw exceptions here.
//   }

//   // If the constructor and destructor are not enough for setting up
//   // and cleaning up each test, you can define the following methods:

//   void SetUp() override {
//      // Code here will be called immediately after the constructor (right
//      // before each test).
//      this->w = new Winch();
//   }

//   void TearDown() override {
//      // Code here will be called immediately after each test (right
//      // before the destructor).
//      this->w->DestroyInstance();
//   }

//   // Class members declared here can be used by all tests in the test suite
//   // for Foo.
// };

// TEST_F(WinchTest, MethodInitialize) {
//   this->w->initialize();
//   EXPECT_EQ(State::INIT, this->w->getState());
// }

// TEST_F(WinchTest, MethodInitializedWithInitialize) {
//   this->w->initialize();
//   this->w->initialized();
//   EXPECT_EQ(State::IDLE, this->w->getState());
// }

// TEST_F(WinchTest, MethodInitializedWithoutInitialize) {
//   this->w->initialized();
//   EXPECT_EQ(State::UNKNOWN, this->w->getState());
// }