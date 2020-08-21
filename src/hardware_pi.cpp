/**
 * @file hardwarepi.cpp
 * @author Mickael GAILLARD (mick.gaillard@gmail.com)
 * @brief OpenWinch Project
 * 
 * @copyright Copyright © 2020
 */

#include "hardware_pi.hpp"
#include "hardware_config.hpp"

#include "bridge_io.hpp"
#include "tachometer.hpp"

#include "controller.hpp"
#include "input.hpp"

#include "logger.hpp"
#include <functional>

Raspberrypi::Raspberrypi(Winch *winch) : Board(winch) {
  this->logger = &Logger::get();
  this->logger->debug("IO : Initialize Raspberry...");

  // Power
  this->power_cmd = new OutputDevice(OUT_PWR);
  this->power_cmd->off();

  // Reverse
  this->reverse_cmd = new OutputDevice(OUT_REVERSE);

  // Speed mode (Lo, Medium, Hi)
  this->speed_cmd = new OutputDevice(OUT_SPD);

  // Throlle
  this->throttle_cmd = new PWMOutputDevice(OUT_THROTTLE);

  // Move
  this->key_enter_btn = new InputDevice(IN_KEY_ENTER);
  this->key_left_btn = new InputDevice(IN_KEY_LEFT);
  this->key_right_btn = new InputDevice(IN_KEY_RIGHT);

  // Register event
  this->key_enter_btn->when_pressed(std::bind(
                                      &Raspberrypi::pressedEnter,
                                      this,
                                      std::placeholders::_1,
                                      std::placeholders::_2,
                                      std::placeholders::_3));

  this->key_left_btn->when_pressed(std::bind(
                                      &Raspberrypi::pressedLeft,
                                      this,
                                      std::placeholders::_1,
                                      std::placeholders::_2,
                                      std::placeholders::_3));

  this->key_right_btn->when_pressed(std::bind(
                                      &Raspberrypi::pressedRight,
                                      this,
                                      std::placeholders::_1,
                                      std::placeholders::_2,
                                      std::placeholders::_3));

  this->tacho = &Tachometer::get();
}

bool Raspberrypi::debounced(uint32_t tick) {
  uint32_t delta = (tick - this->last_tick_btn) / 10000;
  bool result = (delta > 20);
  if (result) { this->last_tick_btn = tick; }
  return result;
}

void Raspberrypi::pressedEnter(int gpio, int level, uint32_t tick) {
  if (this->debounced(tick)) {
    this->logger->debug("IO : Enter pressed !");
    this->winch->enterGui(InputType::ENTER);
  }
}

void Raspberrypi::pressedLeft(int gpio, int level, uint32_t tick) {
  if (this->debounced(tick)) {
    this->logger->debug("IO : Move Left pressed !");
    this->winch->enterGui(InputType::LEFT);
  }
}

void Raspberrypi::pressedRight(int gpio, int level, uint32_t tick) {
if (this->debounced(tick)) {
    this->logger->debug("IO : Move Right pressed !");
    this->winch->enterGui(InputType::RIGHT);
  }
}

void Raspberrypi::initialize() {
  Board::initialize();

  this->setReverse(false);
  this->setSpeedMode(SpeedMode::LOW);
  this->throttle_cmd->setValue(0);
  this->throttle_cmd->on();

  this->power_cmd->on();
  this->logger->info("IO : Hardware Initialized !");
}

void Raspberrypi::emergency() {
  this->logger->debug("IO : Shutdown power !");
  this->power_cmd->off();
}

void Raspberrypi::setThrottleValue(uint8_t value) {
  if (this->throttle_cmd->getValue() != value) {
    this->logger->debug("IO : Throttle to %s", value);
    this->throttle_cmd->setValue(value);
  }
}

uint8_t Raspberrypi::getThrottleValue() {
  return this->throttle_cmd->getValue();
}

void Raspberrypi::setSpeedMode(SpeedMode speed_mode) {
  Board::setSpeedMode(speed_mode);

  if (this->speed_mode == SpeedMode::LOW) {
    this->speed_cmd->off();
  // } else if (this->speed_mode == SpeedMode::MEDIUM) {
  //     this->speed_cmd->off()
  } else if (this->speed_mode == SpeedMode::HIGH) {
    this->speed_cmd->on();
  }
}

void Raspberrypi::setReverse(bool enable) {
  Board::setReverse(enable);

  if (this->reverse) {
    this->reverse_cmd->off();
  } else {
    this->reverse_cmd->on();
  }
}

uint8_t Raspberrypi::getBattery() {
  return 100;
}
