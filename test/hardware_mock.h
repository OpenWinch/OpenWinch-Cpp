/**
 * @file hardware_mock.cpp
 * @author Mickael GAILLARD (mick.gaillard@gmail.com)
 * @brief OpenWinch Project
 * 
 * @copyright Copyright © 2020
 */

#include "hardware.hpp"

#include "gmock/gmock.h"  // Brings in gMock.

class MockBoard : public Board {
 public:
  MockBoard(Winch*) : Board(winch) {};

  MOCK_METHOD(void, initialize, (), (override));
  MOCK_METHOD(void, emergency, (), (override));
  MOCK_METHOD(void, setThrottleValue, (uint8_t value), (override));
  MOCK_METHOD(uint8_t, getThrottleValue, (), (override));
  MOCK_METHOD(void, setReverse, (bool value), (override));
  MOCK_METHOD(void, setSpeedMode, (SpeedMode value), (override));
  MOCK_METHOD(uint8_t, getBattery, (), (override));
};