/**
 * @file state_test.cpp
 * @author Mickael GAILLARD (mick.gaillard@gmail.com)
 * @brief OpenWinch Project
 * 
 * @copyright Copyright © 2020
 */
#include "tachometer.hpp"
#include "tachometer_mock.hpp"
#include <thread>
#include "gtest/gtest.h"

class TachometerTest : public ::testing::Test {
 private:

 protected:

  TachometerTest() {
     // You can do set-up work for each test here.
  }

  ~TachometerTest() override {
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

TEST_F(TachometerTest, MethodCheckRun) {
  MockInputDevice *devU = new MockInputDevice(3, 4, 'U');
  MockInputDevice *devW = new MockInputDevice(3, 2, 'W');
  MockInputDevice *devV = new MockInputDevice(3, 0, 'V');

  Tachometer *tacho = new Tachometer(devU, devW, devV);

  std::cout <<  "cnt\t | VWU "<< std::endl;
  for (size_t i = 0; i < 120; i++) {
    devV->pulse_forward();
    devW->pulse_forward();
    devU->pulse_forward();
   //  std::cout <<  unsigned(i) << "\t | "
   //    << unsigned(devV->digitalRead())
   //    << unsigned(devW->digitalRead())
   //    << unsigned(devU->digitalRead())
   //    << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  std::cout << tacho->get_rpm(tacho->get_hall_sensorV().pulseTime) << std::endl;

//   std::cout <<  "cnt\t | VWU "<< std::endl;
//   for (size_t i = 0; i < 12; i++) {
//     devV->pulse_backward();
//     devW->pulse_backward();
//     devU->pulse_backward();
//     std::cout <<  unsigned(i) << "\t | "
//       << unsigned(devV->digitalRead())
//       << unsigned(devW->digitalRead())
//       << unsigned(devU->digitalRead())
//       << std::endl;
//     std::this_thread::sleep_for(std::chrono::milliseconds(100));
//   }


  // EXPECT_TRUE(State::checkRun(State::RUNNING));
  // EXPECT_TRUE(State::checkRun(State::START));
  // EXPECT_FALSE(State::checkRun(State::UNKNOWN));
}