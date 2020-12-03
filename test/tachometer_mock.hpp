/**
 * @file tachometer_mock.cpp
 * @author Mickael GAILLARD (mick.gaillard@gmail.com)
 * @brief OpenWinch Project
 * 
 * @copyright Copyright © 2020
 */

//#include "gmock/gmock.h"  // Brings in gMock.
#include "tachometer.hpp"
#include "bridge_io.hpp"

#include <iostream>
#include <functional>
#include <chrono>

class MockInputDevice : public InputDevice {
 public:
  MockInputDevice(uint8_t nbSensor, uint8_t posSensor, char nameSensor)
      : InputDevice(0) {

    this->nb = nbSensor;
    this->pos = posSensor;
    this->name = nameSensor;

    if (this->pos <= 0) {
      this->state = 1;
    }

    this->start = std::chrono::high_resolution_clock::now();
    std::cout <<  "Initialise sensor : "
              << this->name
              << "(" << unsigned(this->pos) << ")"
              << std::endl;
  }
  uint8_t digitalRead() { return this->state; }
  uint8_t getCount() { return this->count; }

  void pulse_forward() {
    this->count += 1;
    this->simulate();
  }

  void pulse_backward() {
    this->count -= 1;
    this->simulate();
  }

  void when_pressed(const cb_t &callback) override {
    this->pressedEvent = callback;
  }

  void when_released(const cb_t &callback) override {
    this->releasedEvent = callback;
  }

 private:
  char name;
  uint8_t nb;
  uint16_t pos;
  uint32_t count = 2100000000;
  uint8_t state;
  std::chrono::time_point<std::chrono::system_clock> start;

  void simulate() {
    uint32_t subcnt = this->count + this->pos;
    // std::cout <<  "pulse from : " << this->name << "(" << unsigned(this->pos) << ")"
    //   << " count=" << unsigned(this->count)
    //   << " subcn=" << unsigned(subcnt)
    //   << " old state=" << unsigned(this->state)
    //   << " MOD6=" << unsigned(subcnt % 7)
    //   << " MOD3=" << unsigned(subcnt % 4)
    //   << std::endl;

    if (subcnt % (this->nb * 2) == 0) {
      this->state = 1;

      if (this->pressedEvent != nullptr) {
        int gpio = this->pos;
        int level = 0;
        auto duration = std::chrono::system_clock::now() - this->start;
        uint32_t tick = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
        this->pressedEvent(gpio, level, tick);
      }
    } else if ((subcnt % this->nb == 0)) {
      this->state = 0;

      if (this->releasedEvent != nullptr) {
        int gpio = this->pos;
        int level = 0;
        auto duration = std::chrono::system_clock::now() - this->start;
        uint32_t tick = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
        this->releasedEvent(gpio, level, tick);
      }
    }
  }
};
