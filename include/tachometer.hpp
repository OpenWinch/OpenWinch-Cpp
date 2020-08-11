/*
  tachometer.h - OpenWinch Project

  Copyright (C) 2020  Mickael Gaillard
*/

#ifndef TACHOMETER_HPP_
#define TACHOMETER_HPP_

#include <string>
#include "logger.hpp"

typedef enum rotation {
  Clock = 1,
  CounterClock = -1
} rotation_t;


typedef struct tacho_hallSensor {
    uint32_t pulseTime;
    int pulseCount;

    uint32_t startTime;
    uint32_t prevTime;

    int rpm;
} tacho_hallSensor_t;


class Tachometer {
 public:

  static Tachometer& get() {
    static Tachometer tacho;
    return tacho;
  }

  void hall_debug(tacho_hallSensor_t sensor);

  uint32_t get_rpm(uint32_t pulseTime);

  void int_hall_W();
  void int_hall_V();
  void int_hall_U();

  tacho_hallSensor_t get_hall_sensorU();
  tacho_hallSensor_t get_hall_sensorW();
  tacho_hallSensor_t get_hall_sensorV();

 private:
  Logger *logger = nullptr;
  rotation_t tacho_direct = rotation::Clock;

  tacho_hallSensor_t tacho_hsu;
  tacho_hallSensor_t tacho_hsw;
  tacho_hallSensor_t tacho_hsv;

  Tachometer();
  ~Tachometer()= default;
  Tachometer(const Tachometer&)= delete;
  Tachometer& operator=(const Tachometer&)= delete;

  void hall_init(tacho_hallSensor_t *sensor);
  void error_exit(std::string msg);
};

#endif  // TACHOMETER_HPP_
