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

class MockInputDevice : public InputDevice {
 public:
  MockInputDevice(uint8_t nbSensor, uint8_t posSensor, char nameSensor) : InputDevice(0) {
    this->nb = nbSensor;
    this->pos = posSensor;
    this->name = nameSensor;

    if (this->pos <= 0) {
      this->state = 1;
    }

    std::cout <<  "Initialise sensor : " << this->name << "(" << unsigned(this->pos) << ")" << std::endl;
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
    std::cout << "IO: Set pressed callback : " << std::endl;  // reinterpret_cast<unsigned char *>(callback) << std::endl;
    this->pressedEvent = callback;
  }

  void when_released(const cb_t &callback) override {
    std::cout << "IO: Set released callback : " << std::endl;  // reinterpret_cast<unsigned char *>(callback) << std::endl;

    this->releasedEvent = callback;
  }

 private:
  char name;
  uint8_t nb;
  uint8_t pos;
  uint8_t count = 0;
  uint8_t state;

  void simulate() {
    uint8_t subcnt = this->count + this->pos;
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
        uint32_t tick = this->count;
        this->pressedEvent(gpio, level, tick);
      }
    } else if ((subcnt % this->nb == 0)) {
      this->state = 0;

      if (this->releasedEvent != nullptr) {
        int gpio = this->pos;
        int level = 0;
        uint32_t tick = this->count;
        this->releasedEvent(gpio, level, tick);
      }
    }
  }
};
