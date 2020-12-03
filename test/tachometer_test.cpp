/**
 * @file state_test.cpp
 * @author Mickael GAILLARD (mick.gaillard@gmail.com)
 * @brief OpenWinch Project
 * 
 * @copyright Copyright © 2020
 */

#include "gtest/gtest.h"

#include <thread>

#include "constantes.hpp"
#include "tachometer.hpp"
#include "bridge_mock.hpp"


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

    std::cout
      << "Avg Pulse : " << unsigned(this->tacho->get_hall_sensorV().pulseTime)
      << " RPM : " << unsigned(this->tacho->get_rpm())
      << std::endl;

    EXPECT_LE(rpm - variation, this->tacho->get_rpm());
    EXPECT_GE(rpm + variation, this->tacho->get_rpm());

    EXPECT_LE(rpm - variation, this->tacho->get_rpmV());
    EXPECT_GE(rpm + variation, this->tacho->get_rpmV());

    EXPECT_LE(rpm - variation, this->tacho->get_rpmW());
    EXPECT_GE(rpm + variation, this->tacho->get_rpmW());

    EXPECT_LE(rpm - variation, this->tacho->get_rpmU());
    EXPECT_GE(rpm + variation, this->tacho->get_rpmU());
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

    std::cout
      << "Avg Pulse : " << unsigned(this->tacho->get_hall_sensorV().pulseTime)
      << " RPM : " << unsigned(this->tacho->get_rpm())
      << std::endl;

    EXPECT_LE(rpm - variation, this->tacho->get_rpm());
    EXPECT_GE(rpm + variation, this->tacho->get_rpm());

    EXPECT_LE(rpm - variation, this->tacho->get_rpmV());
    EXPECT_GE(rpm + variation, this->tacho->get_rpmV());

    EXPECT_LE(rpm - variation, this->tacho->get_rpmW());
    EXPECT_GE(rpm + variation, this->tacho->get_rpmW());

    EXPECT_LE(rpm - variation, this->tacho->get_rpmU());
    EXPECT_GE(rpm + variation, this->tacho->get_rpmU());
    EXPECT_EQ(rotation_t::CounterClock, this->tacho->get_rotation());
  }
};


// TEST_F(TachometerTest, DISABLE_MethodSingletonInstance) {
//   Tachometer *tacho = &Tachometer::get();
//   EXPECT_NE(&tacho, NULL);
// }

// TEST_F(TachometerTest, MethodError) {
//   EXPECT_EXIT(this->tacho->error_exit("test"), ::testing::ExitedWithCode(1), "test");
// }

TEST_F(TachometerTest, MethodHallDebug) {
  this->tacho->hall_debug(this->tacho->get_hall_sensorV(), "V");
  EXPECT_TRUE(true);
}

TEST_F(TachometerTest, MethodReduceMotor) {
  const uint32_t RPM = 100;
  this->TestMethodForward(RPM, 20, 300);

  uint32_t actual = this->tacho->reduce_rpm(this->tacho->get_rpm());
  uint32_t expected = RPM/MOTOR_REDUCE;

  EXPECT_LE(expected - 10, actual);
  EXPECT_GE(expected + 10, actual);
}

TEST_F(TachometerTest, MethodGetHallV) {
  for (size_t i = 0; i < 100; i++) {
      this->devV->pulse_forward();
      this->devW->pulse_forward();
      this->devU->pulse_forward();
  }
  auto hall = this->tacho->get_hall_sensorV();
  EXPECT_EQ(hall.pulseCount, 100/3);
}

TEST_F(TachometerTest, MethodGetHallU) {
  for (size_t i = 0; i < 100; i++) {
      this->devV->pulse_forward();
      this->devW->pulse_forward();
      this->devU->pulse_forward();
  }
  auto hall = this->tacho->get_hall_sensorU();
  EXPECT_EQ(hall.pulseCount, 100/3);
}

TEST_F(TachometerTest, MethodGetHallW) {
  for (size_t i = 0; i < 100; i++) {
      this->devV->pulse_forward();
      this->devW->pulse_forward();
      this->devU->pulse_forward();
  }
  auto hall = this->tacho->get_hall_sensorW();
  EXPECT_EQ(hall.pulseCount, (100/3)+1);
}

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
  this->TestMethodForward(RPM, 350, 20000);
}

TEST_F(TachometerTest, MethodBackward6000) {
  const uint32_t RPM = 6000;
  this->TestMethodBackward(RPM, 350, 20000);
}

/******************************************************************************/
// RPM 3000
/******************************************************************************/

TEST_F(TachometerTest, MethodForward3000) {
  const uint32_t RPM = 3000;
  this->TestMethodForward(RPM, 250, 10000);
}

TEST_F(TachometerTest, MethodBackward3000) {
  const uint32_t RPM = 3000;
  this->TestMethodBackward(RPM, 250, 10000);
}

/******************************************************************************/
// RPM 1000
/******************************************************************************/

TEST_F(TachometerTest, MethodForward1000) {
  const uint32_t RPM = 1000;
  this->TestMethodForward(RPM, 150, 1200);
}

TEST_F(TachometerTest, MethodBackward1000) {
  const uint32_t RPM = 1000;
  this->TestMethodBackward(RPM, 150, 1200);
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
  this->TestMethodForward(RPM, 10, 25);
}

TEST_F(TachometerTest, MethodBackward10) {
  const uint32_t RPM = 10;
  this->TestMethodBackward(RPM, 10, 25);
}
