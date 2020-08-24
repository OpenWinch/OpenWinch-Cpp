/**
 * @file tachometer.cpp
 * @author Mickael GAILLARD (mick.gaillard@gmail.com)
 * @brief OpenWinch Project
 * 
 * @copyright Copyright © 2020
 */

// gcc -Wall -o tachometer tachometer.cpp.c -lwiringPi -O3

#include <stdint.h>
#include <errno.h>
#include <stdlib.h>

#include <cstdio>
#include <cstring>
#include <ctime>
#include <sstream>
#include <string>
#include <iostream>
#include <functional>


#include "tachometer.hpp"
#include "bridge_io.hpp"
#include "constantes.hpp"

#include "hardware_config.hpp"
#include "logger.hpp"


Tachometer::Tachometer() {
  this->logger = &Logger::get();
  this->logger->debug("IO-T : Init Tachometer...");

  this->initialize();

  // Sensor U
  this->input_hal_u = new InputDevice(IN_HS_U);
  this->input_hal_u->when_pressed(std::bind(
                                  &Tachometer::int_hall_U,
                                  this,
                                  std::placeholders::_1,
                                  std::placeholders::_2,
                                  std::placeholders::_3));

  // Sensor W
  this->input_hal_w = new InputDevice(IN_HS_W);
  this->input_hal_w->when_pressed(std::bind(
                                  &Tachometer::int_hall_W,
                                  this,
                                  std::placeholders::_1,
                                  std::placeholders::_2,
                                  std::placeholders::_3));

  // Sensor V
  this->input_hal_v = new InputDevice(IN_HS_V);
  this->input_hal_v->when_pressed(std::bind(
                                  &Tachometer::int_hall_V,
                                  this,
                                  std::placeholders::_1,
                                  std::placeholders::_2,
                                  std::placeholders::_3));
}

void Tachometer::initialize() {
  this->logger->debug("IO-T : Reset Tachometer !");

  this->hall_init(&this->tacho_hsu);
  this->hall_init(&this->tacho_hsw);
  this->hall_init(&this->tacho_hsv);
}

void Tachometer::int_hall_W(int gpio, int level, uint32_t tick) {
#ifdef OW_TA_DEBUG
    std::cout << "IO-T : Pulse W" << std::endl;
#endif
    // Set startTime to current microcontroller elapsed time value
    this->tacho_hsw.startTime = tick;

    // Read the current W hall sensor value
    uint8_t HSW_Val = this->input_hal_w->digitalRead();
    // Read the current V (or U) hall sensor value
    uint8_t HSV_Val = this->input_hal_v->digitalRead();
    // Determine rotation __direction (ternary if statement)
    this->tacho_direct = HSW_Val == HSV_Val ? Clock : CounterClock;

    // Calculate the current time between pulses W
    this->tacho_hsw.pulseTime = this->tacho_hsw.startTime - this->tacho_hsw.prevTime;
    // Remember the start time for the next interrupt
    this->tacho_hsw.prevTime = this->tacho_hsw.startTime;

    // Calculate Rotation Per Minute
    //rpmW = get_rpm(pulseTimeW);
    this->tacho_hsw.pulseCount = this->tacho_hsw.pulseCount + (1 * this->tacho_direct);
}

void Tachometer::int_hall_V(int gpio, int level, uint32_t tick) {
#ifdef OW_TA_DEBUG
    std::cout << "IO-T : Pulse V" << std::endl;
#endif
    this->tacho_hsv.startTime = tick;

    int HSV_Val = this->input_hal_v->digitalRead();
    int HSU_Val = this->input_hal_u->digitalRead();
    this->tacho_direct = HSV_Val == HSU_Val ? Clock : CounterClock;

    this->tacho_hsv.pulseTime = this->tacho_hsv.startTime - this->tacho_hsv.prevTime;
    this->tacho_hsv.prevTime = this->tacho_hsv.startTime;

    //self.rpmV = self.__get_rpm(self.pulseTimeV);
    this->tacho_hsv.pulseCount = this->tacho_hsv.pulseCount + (1 * this->tacho_direct);
}

void Tachometer::int_hall_U(int gpio, int level, uint32_t tick) {
#ifdef OW_TA_DEBUG
    std::cout << "IO-T : Pulse U" << std::endl;
#endif
    this->tacho_hsu.startTime = tick;

    int HSU_Val = this->input_hal_u->digitalRead();
    int HSW_Val = this->input_hal_w->digitalRead();
    this->tacho_direct = HSU_Val == HSW_Val ? Clock : CounterClock;

    this->tacho_hsu.pulseTime = this->tacho_hsu.startTime - this->tacho_hsu.prevTime;
    this->tacho_hsu.prevTime = this->tacho_hsu.startTime;

    //this->rpmU = self.__get_rpm(self.pulseTimeU)
    this->tacho_hsu.pulseCount = this->tacho_hsu.pulseCount + (1 * this->tacho_direct);
}

uint32_t Tachometer::get_rpm(uint32_t pulseTime) {
  uint32_t RPM = 0;

  if (pulseTime != 0) {
    // Calculate the pulses per min (1000 millis in 1 second)
    // (1000 / pulseTime) * 60
    uint32_t PPM = (60000 / pulseTime);
    // Calculate revs per minute based on number of pulses per rev
    RPM = PPM / (MOTOR_PPR / 3);
  }

  return RPM;
}

void Tachometer::hall_init(tacho_hallSensor_t *sensor) {
  this->logger->debug("IO-T : Init Sensor...");

  sensor->pulseTime = 0;
  sensor->pulseCount = 0;

  sensor->startTime = 0;
  sensor->prevTime = 0;

  sensor->rpm = 0;
}

void Tachometer::hall_debug(tacho_hallSensor_t sensor, std::string name) {
  this->logger->debug("{ name=%s time=%d, count=%d, rpm=%d }",
                      name.c_str(),
                      sensor.pulseTime,
                      sensor.pulseCount,
                      sensor.rpm);
}

void Tachometer::error_exit(std::string msg) {
  std::stringstream ss;
  ss << msg << std::strerror(errno) << std::endl;
  std::string s = ss.str();

  this->logger->fatal(s.c_str());
  std::cerr << s;

  exit(1);
}

tacho_hallSensor_t Tachometer::get_hall_sensorU() {
  return this->tacho_hsu;
}

tacho_hallSensor_t Tachometer::get_hall_sensorW() {
  return this->tacho_hsw;
}

tacho_hallSensor_t Tachometer::get_hall_sensorV() {
  return this->tacho_hsv;
}
