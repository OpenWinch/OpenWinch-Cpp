/**
 * @file tachometer.cpp
 * @author Mickael GAILLARD (mick.gaillard@gmail.com)
 * @brief OpenWinch Project
 * 
 * @copyright Copyright © 2020-2021
 */

#include "openwinch.hpp"
#include "tachometer.hpp"

#include "bridge_io.hpp"

#include <cstring>
#include <sstream>
#include <string>
#include <iostream>
#include <functional>

Tachometer& Tachometer::get() {
  static Tachometer tacho(
    new InputDevice(IN_HS_U),
    new InputDevice(IN_HS_W),
    new InputDevice(IN_HS_V));
  return tacho;
}

Tachometer::Tachometer(InputDevice *sensor_u, InputDevice *sensor_w, InputDevice *sensor_v) {
  this->logger = &Logger::get();
  this->logger->debug("IO-T : Init Tachometer...");

  this->initialize();

  // Sensor U
  this->input_hal_u = sensor_u;
  this->input_hal_u->when_pressed(std::bind(
                                  &Tachometer::int_hall_U,
                                  this,
                                  std::placeholders::_1,
                                  std::placeholders::_2,
                                  std::placeholders::_3));

  this->input_hal_u->when_released(std::bind(
                                  &Tachometer::int_hall_U,
                                  this,
                                  std::placeholders::_1,
                                  std::placeholders::_2,
                                  std::placeholders::_3));

  // Sensor W
  this->input_hal_w = sensor_w;
  this->input_hal_w->when_pressed(std::bind(
                                  &Tachometer::int_hall_W,
                                  this,
                                  std::placeholders::_1,
                                  std::placeholders::_2,
                                  std::placeholders::_3));

  this->input_hal_w->when_released(std::bind(
                                  &Tachometer::int_hall_W,
                                  this,
                                  std::placeholders::_1,
                                  std::placeholders::_2,
                                  std::placeholders::_3));

  // Sensor V
  this->input_hal_v = sensor_v;
  this->input_hal_v->when_pressed(std::bind(
                                  &Tachometer::int_hall_V,
                                  this,
                                  std::placeholders::_1,
                                  std::placeholders::_2,
                                  std::placeholders::_3));

  this->input_hal_v->when_released(std::bind(
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

void Tachometer::int_hall_W(int /*gpio*/, int level, uint32_t tick) {
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
    this->tacho_direct = HSW_Val != HSV_Val ? Clock : CounterClock;

    // Calculate the current time between pulses W
    this->tacho_hsw.pulseTime = this->tacho_hsw.startTime - this->tacho_hsw.prevTime;
    // Count number of pulse
    this->tacho_hsw.pulseCount = this->tacho_hsw.pulseCount + (1 * this->tacho_direct);
    // Remember the start time for the next interrupt
    this->tacho_hsw.prevTime = this->tacho_hsw.startTime;
}

void Tachometer::int_hall_V(int /*gpio*/, int level, uint32_t tick) {
#ifdef OW_TA_DEBUG
    std::cout << "IO-T : Pulse V" << std::endl;
#endif
    this->tacho_hsv.startTime = tick;

    int HSV_Val = this->input_hal_v->digitalRead();
    int HSU_Val = this->input_hal_u->digitalRead();
    this->tacho_direct = HSV_Val != HSU_Val ? Clock : CounterClock;

    this->tacho_hsv.pulseTime = this->tacho_hsv.startTime - this->tacho_hsv.prevTime;
    this->tacho_hsv.pulseCount = this->tacho_hsv.pulseCount + (1 * this->tacho_direct);
    this->tacho_hsv.prevTime = this->tacho_hsv.startTime;
}

void Tachometer::int_hall_U(int /*gpio*/, int level, uint32_t tick) {
#ifdef OW_TA_DEBUG
    std::cout << "IO-T : Pulse U" << std::endl;
#endif
    this->tacho_hsu.startTime = tick;

    int HSU_Val = this->input_hal_u->digitalRead();
    int HSW_Val = this->input_hal_w->digitalRead();
    this->tacho_direct = HSU_Val != HSW_Val ? Clock : CounterClock;

    this->tacho_hsu.pulseTime = this->tacho_hsu.startTime - this->tacho_hsu.prevTime;
    this->tacho_hsu.pulseCount = this->tacho_hsu.pulseCount + (1 * this->tacho_direct);
    this->tacho_hsu.prevTime = this->tacho_hsu.startTime;
}

uint32_t Tachometer::get_rpm(uint32_t pulseTime) {
  uint32_t rpm = 0;

  if (pulseTime != 0) {
    // Calculate the pulses per min (1000000 microSecondes in 1 second)
    // (1000000 / pulseTime) * 60
    const uint32_t ppm = (60000000 / pulseTime);
    // Calculate revs per minute based on number of pulses per rev
    rpm = ppm / MOTOR_PPR;
  }

  return rpm;
}

uint32_t Tachometer::get_rpm() {
  int32_t rpmU = this->get_rpmU();
  int32_t rpmW = this->get_rpmW();
  int32_t rpmV = this->get_rpmV();

  // Get Median value (no mean for outle)
  if ((rpmU - rpmW) * (rpmV - rpmU) >= 0) {
      return rpmU;
  } else if ((rpmW - rpmU) * (rpmV - rpmW) >= 0) {
      return rpmW;
  } else {
      return rpmV;
  }
}

uint32_t Tachometer::reduce_rpm(uint32_t rpm_motor) {
  return rpm_motor/MOTOR_REDUCE;
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

rotation_t Tachometer::get_rotation() {
  return this->tacho_direct;
}

uint32_t Tachometer::get_rpmU() {
  return this->get_rpm(this->tacho_hsu.pulseTime);
}

uint32_t Tachometer::get_rpmW() {
  return this->get_rpm(this->tacho_hsw.pulseTime);
}

uint32_t Tachometer::get_rpmV() {
  return this->get_rpm(this->tacho_hsv.pulseTime);
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
