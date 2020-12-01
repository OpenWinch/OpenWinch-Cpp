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
 protected:
  const uint8_t nb_sensor = 3;
  MockInputDevice *devU = nullptr;
  MockInputDevice *devW = nullptr;
  MockInputDevice *devV = nullptr;

  Tachometer *tacho = nullptr;

  TachometerTest() {
     // You can do set-up work for each test here.
  }

  ~TachometerTest() override {
     // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  void SetUp() override {
    this->devU = new MockInputDevice(nb_sensor, 4, 'U');
    this->devW = new MockInputDevice(nb_sensor, 2, 'W');
    this->devV = new MockInputDevice(nb_sensor, 0, 'V');

    this->tacho = new Tachometer(devU, devW, devV);
  }

  void TearDown() override {
     delete this->tacho;
     this->tacho = nullptr;

     delete this->devU;
     delete this->devW;
     delete this->devV;

     this->devU = nullptr;
     this->devW = nullptr;
     this->devV = nullptr;
  }

  // Class members declared here can be used by all tests in the test suite
  // for Foo.
};

TEST_F(TachometerTest, MethodForwardBakward) {
  const int spd = 10;

  std::cout <<  "cnt\t | VWU "<< std::endl;
  for (size_t i = 0; i < 12; i++) {
    this->devV->pulse_forward();
    this->devW->pulse_forward();
    this->devU->pulse_forward();
    std::cout <<  unsigned(i) << "\t | "
      << unsigned(devV->digitalRead())
      << unsigned(devW->digitalRead())
      << unsigned(devU->digitalRead())
      << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(spd));
  }

  std::cout <<  "cnt\t | VWU "<< std::endl;
  for (size_t i = 0; i < 12; i++) {
    this->devV->pulse_backward();
    this->devW->pulse_backward();
    this->devU->pulse_backward();
    std::cout <<  unsigned(i) << "\t | "
      << unsigned(devV->digitalRead())
      << unsigned(devW->digitalRead())
      << unsigned(devU->digitalRead())
      << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(spd));
  }
}

/******************************************************************************/
// RPM 66
/******************************************************************************/

TEST_F(TachometerTest, MethodForward66) {
  const int spd = 10;
  const int iter = 120;

  for (size_t i = 0; i < iter; i++) {
    this->devV->pulse_forward();
    this->devW->pulse_forward();
    this->devU->pulse_forward();

    std::this_thread::sleep_for(std::chrono::milliseconds(spd));
  }

  uint32_t actual = this->tacho->get_rpm(this->tacho->get_hall_sensorV().pulseTime);
  uint32_t ref = (60000/(spd*nb_sensor*2)/(45/3));
  EXPECT_LE(ref - spd, actual);
  EXPECT_GE(ref + spd, actual);
  EXPECT_EQ(rotation_t::Clock, this->tacho->get_rotation());
}

TEST_F(TachometerTest, MethodBackward66) {
  const int spd = 10;
  const int iter = 120;

  for (size_t i = 0; i < iter; i++) {
    this->devV->pulse_backward();
    this->devW->pulse_backward();
    this->devU->pulse_backward();

    std::this_thread::sleep_for(std::chrono::milliseconds(spd));
  }

  uint32_t actual = this->tacho->get_rpm(this->tacho->get_hall_sensorV().pulseTime);
  uint32_t ref = (60000/(spd*nb_sensor*2)/(45/3));
  EXPECT_LE(ref - spd, actual);
  EXPECT_GE(ref + spd, actual);
  EXPECT_EQ(rotation_t::CounterClock, this->tacho->get_rotation());
}

/******************************************************************************/
// RPM 5000
/******************************************************************************/

TEST_F(TachometerTest, MethodForward67) {
  const int spd = 100000;
  const int iter = 120;

  for (size_t i = 0; i < iter; i++) {
    this->devV->pulse_forward();
    this->devW->pulse_forward();
    this->devU->pulse_forward();

    std::this_thread::sleep_for(std::chrono::nanoseconds(spd));
  }

  uint32_t actual = this->tacho->get_rpm(this->tacho->get_hall_sensorV().pulseTime);
  uint32_t ref = ((60000*1000000)/(spd*nb_sensor*2)/(45/3));
  EXPECT_LE(ref - spd, actual);
  EXPECT_GE(ref + spd, actual);
  EXPECT_EQ(rotation_t::Clock, this->tacho->get_rotation());
}

TEST_F(TachometerTest, MethodBackward67) {
  const int spd = 100000;
  const int iter = 120;

  for (size_t i = 0; i < iter; i++) {
    this->devV->pulse_backward();
    this->devW->pulse_backward();
    this->devU->pulse_backward();

    std::this_thread::sleep_for(std::chrono::nanoseconds(spd));
  }

  uint32_t actual = this->tacho->get_rpm(this->tacho->get_hall_sensorV().pulseTime);
  uint32_t ref = ((60000*1000000)/(spd*nb_sensor*2)/(45/3));
  EXPECT_LE(ref - spd, actual);
  EXPECT_GE(ref + spd, actual);
  EXPECT_EQ(rotation_t::CounterClock, this->tacho->get_rotation());
}