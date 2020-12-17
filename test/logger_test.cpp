/**
 * @file logger_test.cpp
 * @author Mickael GAILLARD (mick.gaillard@gmail.com)
 * @brief OpenWinch Project
 * 
 * @copyright Copyright © 2020
 */
#include "logger.hpp"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

class LoggerTest : public ::testing::Test {
 protected:
  Logger *logger = nullptr;

  LoggerTest() { }
  ~LoggerTest() override { }

  void SetUp() override {
    this->logger = &Logger::get();
  }
  void TearDown() override { }
};

TEST_F(LoggerTest, MethodLive) {
  this->logger->live("Log live.");
}

TEST_F(LoggerTest, MethodInfo) {
  this->logger->info("Log info.");
}

TEST_F(LoggerTest, MethodDebug) {
  this->logger->debug("Log debug.");
}

TEST_F(LoggerTest, MethodWarning) {
  this->logger->warning("Log warning.");
}

TEST_F(LoggerTest, MethodError) {
  this->logger->error("Log error.");
}

TEST_F(LoggerTest, MethodFatal) {
  this->logger->fatal("Log fatal.");
}
