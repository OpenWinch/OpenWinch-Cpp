 /**
 * @file input_test.cpp
 * @author Mickael GAILLARD (mick.gaillard@gmail.com)
 * @brief OpenWinch Project
 * 
 * @copyright Copyright © 2020-2021
 */
#include "input.hpp"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

class InputTypeTest : public ::testing::Test {
 private:

 protected:
  InputTypeTest() { }
  ~InputTypeTest() override { }

  void SetUp() override { }
  void TearDown() override { }
};

TEST_F(InputTypeTest, MethodToString) {
  EXPECT_STRCASEEQ("UP", std::string(InputType(InputType::UP)).c_str());
  EXPECT_STRCASEEQ("RIGHT", std::string(InputType(InputType::RIGHT)).c_str());
  EXPECT_STRCASEEQ("DOWN", std::string(InputType(InputType::DOWN)).c_str());
  EXPECT_STRCASEEQ("LEFT", std::string(InputType(InputType::LEFT)).c_str());
  EXPECT_STRCASEEQ("DOWN", std::string(InputType(InputType::ENTER)).c_str());
  EXPECT_STRCASEEQ("NONE", std::string(InputType(InputType::NONE)).c_str());
}

TEST_F(InputTypeTest, MethodOperator) {
  EXPECT_TRUE(InputType(InputType::UP) == InputType(InputType::UP));
  EXPECT_FALSE(InputType(InputType::UP) == InputType(InputType::DOWN));

  EXPECT_TRUE(InputType(InputType::UP) != InputType(InputType::DOWN));
  EXPECT_FALSE(InputType(InputType::UP) != InputType(InputType::UP));
}