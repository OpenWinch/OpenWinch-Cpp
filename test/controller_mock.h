/**
 * @file controller_mock.cpp
 * @author Mickael GAILLARD (mick.gaillard@gmail.com)
 * @brief OpenWinch Project
 * 
 * @copyright Copyright © 2020
 */

#include "gmock/gmock.h"  // Brings in gMock.
#include "controller.hpp"

class MockWinch : public Winch {
 public:
  MockWinch() {}
  // ...
  // MOCK_METHOD(void, PenUp, (), (override));
  // MOCK_METHOD(void, PenDown, (), (override));
  // MOCK_METHOD(void, Forward, (int distance), (override));
  // MOCK_METHOD(void, Turn, (int degrees), (override));
  // MOCK_METHOD(void, GoTo, (int x, int y), (override));
  // MOCK_METHOD(int, GetX, (), (const, override));
  // MOCK_METHOD(int, GetY, (), (const, override));
};