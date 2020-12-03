/**
 * @file state_test.cpp
 * @author Mickael GAILLARD (mick.gaillard@gmail.com)
 * @brief OpenWinch Project
 * 
 * @copyright Copyright © 2020
 */
#include "constantes.hpp"
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
    this->devU = new MockInputDevice(this->nb_sensor, 4, 'U');
    this->devW = new MockInputDevice(this->nb_sensor, 2, 'W');
    this->devV = new MockInputDevice(this->nb_sensor, 0, 'V');

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

  void TestMethodForward(const uint32_t rpm,
                         const uint16_t variation,
                         const uint16_t iter) {

    const uint32_t mSPP = ((60000000/rpm)/MOTOR_PPR)/this->nb_sensor;

    std::cout << "RPM : " << unsigned(rpm)
      << " Microsecond by pulse : " << mSPP
      << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    auto const start_time = std::chrono::steady_clock::now();
    auto const wait_time = std::chrono::microseconds{mSPP};
    auto next_time = start_time + wait_time;
    for (size_t i = 0; i < iter; i++) {
      this->devV->pulse_forward();
      this->devW->pulse_forward();
      this->devU->pulse_forward();

      std::this_thread::sleep_until(next_time);
      next_time += wait_time;
    }

    auto duration = std::chrono::system_clock::now() - start;
    std::cout << "Duration : " << unsigned(duration.count()) << std::endl;

    uint32_t actual =
      this->tacho->get_rpm(this->tacho->get_hall_sensorV().pulseTime);

    std::cout 
      << "Avg Pulse : " << unsigned(this->tacho->get_hall_sensorV().pulseTime)
      << " RPM : " << unsigned(actual)
      << std::endl;

    EXPECT_LE(rpm - variation, actual);
    EXPECT_GE(rpm + variation, actual);
    EXPECT_EQ(rotation_t::Clock, this->tacho->get_rotation());
  }

  void TestMethodBackward(const uint32_t rpm,
                          const uint16_t variation,
                          const uint16_t iter) {

    const uint32_t mSPP = ((60000000/rpm)/MOTOR_PPR)/this->nb_sensor;

    std::cout << "RPM : " << unsigned(rpm)
      << " Microsecond by pulse : " << mSPP
      << std::endl;

    auto const start_time = std::chrono::steady_clock::now();
    auto const wait_time = std::chrono::microseconds{mSPP};
    auto next_time = start_time + wait_time;
    for (size_t i = 0; i < iter; i++) {
      this->devV->pulse_backward();
      this->devW->pulse_backward();
      this->devU->pulse_backward();

      std::this_thread::sleep_until(next_time);
      next_time += wait_time;
    }

    uint32_t actual =
        this->tacho->get_rpm(this->tacho->get_hall_sensorV().pulseTime);

    std::cout 
      << "Avg Pulse : " << unsigned(this->tacho->get_hall_sensorV().pulseTime)
      << " RPM : " << unsigned(actual)
      << std::endl;

    EXPECT_LE(rpm - variation, actual);
    EXPECT_GE(rpm + variation, actual);
    EXPECT_EQ(rotation_t::CounterClock, this->tacho->get_rotation());
  }
};

/******************************************************************************/

TEST_F(TachometerTest, MethodForwardBakward) {
  const int spd = 10;

  std::cout <<  "cnt\t | VWU " << std::endl << "----------" << std::endl;
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

  std::cout <<  "cnt\t | VWU " << std::endl << "----------" << std::endl;
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
// RPM 6000
/******************************************************************************/

TEST_F(TachometerTest, MethodForward6000) {
  const uint32_t RPM = 6000;
  this->TestMethodForward(RPM, 200, 2400);
}

TEST_F(TachometerTest, MethodBackward6000) {
  const uint32_t RPM = 6000;
  this->TestMethodBackward(RPM, 200, 2400);
}

/******************************************************************************/
// RPM 3000
/******************************************************************************/

TEST_F(TachometerTest, MethodForward3000) {
  const uint32_t RPM = 3000;
  this->TestMethodForward(RPM, 200, 2400);
}

TEST_F(TachometerTest, MethodBackward3000) {
  const uint32_t RPM = 3000;
  this->TestMethodBackward(RPM, 200, 2400);
}

/******************************************************************************/
// RPM 1000
/******************************************************************************/

TEST_F(TachometerTest, MethodForward1000) {
  const uint32_t RPM = 1000;
  this->TestMethodForward(RPM, 100, 1200);
}

TEST_F(TachometerTest, MethodBackward1000) {
  const uint32_t RPM = 1000;
  this->TestMethodBackward(RPM, 100, 1200);
}

/******************************************************************************/
// RPM 100
/******************************************************************************/

TEST_F(TachometerTest, MethodForward100) {
  const uint32_t RPM = 100;
  this->TestMethodForward(RPM, 20, 300);
}

TEST_F(TachometerTest, MethodBackward100) {
  const uint32_t RPM = 100;
  this->TestMethodBackward(RPM, 20, 300);
}

/******************************************************************************/
// RPM 10
/******************************************************************************/

TEST_F(TachometerTest, MethodForward10) {
  const uint32_t RPM = 10;
  this->TestMethodForward(RPM, 10, 100);
}

TEST_F(TachometerTest, MethodBackward10) {
  const uint32_t RPM = 10;
  this->TestMethodBackward(RPM, 10, 100);
}
