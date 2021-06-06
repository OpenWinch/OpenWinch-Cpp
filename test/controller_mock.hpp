/**
 * @file controller_mock.cpp
 * @author Mickael GAILLARD (mick.gaillard@gmail.com)
 * @brief OpenWinch Project
 * 
 * @copyright Copyright © 2020-2021
 */

#include "gmock/gmock.h"  // Brings in gMock.
#include "controller.hpp"
#include "input.hpp"
#include "mode.hpp"

class TestWinch : public Winch {
  State getState() override { return State::UNKNOWN; }
};

class MockWinch : public TestWinch {
 public:
  MockWinch() {}

  MOCK_METHOD(void, boot,         (), (override));
  MOCK_METHOD(void, initialize,   (), (override));
  MOCK_METHOD(void, initialized,  (), (override));
  MOCK_METHOD(void, start,        (), (override));
  MOCK_METHOD(void, started,      (), (override));
  MOCK_METHOD(void, stop,         (), (override));
  MOCK_METHOD(void, stopped,      (), (override));
  MOCK_METHOD(void, emergency,    (), (override));
  MOCK_METHOD(void, display,      (), (override));
  MOCK_METHOD(void, exit,         (), (override));
  //MOCK_METHOD(ModeType, getMode,  (), (override));
  MOCK_METHOD(uint8_t, getSpeedTarget,(), (override));
  MOCK_METHOD(State, getState,    (), (override));
  MOCK_METHOD(uint8_t, getBattery,(), (override));
  MOCK_METHOD(uint8_t, getRemote, (), (override));
  MOCK_METHOD(float, getDistance, (), (override));
  MOCK_METHOD(void, speedUp,      (uint8_t), (override));
  MOCK_METHOD(void, speedDown,    (uint8_t), (override));
  MOCK_METHOD(void, speedValue,   (uint8_t), (override));
  //MOCK_METHOD(void, enterGui,     (InputType), (override));
  MOCK_METHOD(void, manualForward,(), (override));
  MOCK_METHOD(void, manualReverse,(), (override));

  ModeType getMode() {
    return ModeType::OneWay;
  }

  void enterGui(InputType value) {

  }
};