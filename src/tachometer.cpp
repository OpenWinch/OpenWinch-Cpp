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

#include <wiringPi.h>

#include "tachometer.hpp"
#include "logger.hpp"

#define IN_HS_W 1
#define IN_HS_V 2
#define IN_HS_U 3

#define MOTOR_PPR 45      // pulse by turn
#define WINCH_DIAM 0.20   // In meter


void __challSensorW() {
  Tachometer::get().int_hall_W();
}

void __challSensorV() {
  Tachometer::get().int_hall_V();
}

void __challSensorU() {
  Tachometer::get().int_hall_U();
}

Tachometer::Tachometer() {
  this->logger = &Logger::get();
  this->logger->debug("Init GPIO...");

  this->hall_init(&this->tacho_hsu);
  this->hall_init(&this->tacho_hsw);
  this->hall_init(&this->tacho_hsv);

  if (wiringPiSetup() < 0) {
    error_exit("Unable to setup wiringPi: ");
  }

  // Sensor U
  pinMode(IN_HS_U, INPUT);
  // pullUpDnControl(IN_HS_U, PUD_UP);
  if (wiringPiISR(IN_HS_U, INT_EDGE_FALLING, &__challSensorU) < 0) {
    error_exit("Unable to setup ISR U: ");
  }

  // Sensor W
  pinMode(IN_HS_W, INPUT);
  // pullUpDnControl(IN_HS_W, PUD_UP);
  if (wiringPiISR(IN_HS_W, INT_EDGE_FALLING, &__challSensorW) < 0) {
    error_exit("Unable to setup ISR W: ");
  }

  // Sensor V
  pinMode(IN_HS_V, INPUT);
  // pullUpDnControl(IN_HS_V, PUD_UP);
  if (wiringPiISR(IN_HS_V, INT_EDGE_FALLING, &__challSensorV) < 0) {
    error_exit("Unable to setup ISR V: ");
  }

}

void Tachometer::int_hall_W() {
    this->logger->debug("Pulse W");
    // Set startTime to current microcontroller elapsed time value
    this->tacho_hsw.startTime = millis();

    // Read the current W hall sensor value
    int HSW_Val = digitalRead(IN_HS_W);
    // Read the current V (or U) hall sensor value
    int HSV_Val = digitalRead(IN_HS_V);
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

void Tachometer::int_hall_V() {
    this->logger->debug("Pulse V");
    this->tacho_hsv.startTime = millis();

    int HSV_Val = digitalRead(IN_HS_V);
    int HSU_Val = digitalRead(IN_HS_U);
    this->tacho_direct = HSV_Val == HSU_Val ? Clock : CounterClock;

    this->tacho_hsv.pulseTime = this->tacho_hsv.startTime - this->tacho_hsv.prevTime;
    this->tacho_hsv.prevTime = this->tacho_hsv.startTime;

    //self.rpmV = self.__get_rpm(self.pulseTimeV);
    this->tacho_hsv.pulseCount = this->tacho_hsv.pulseCount + (1 * this->tacho_direct);
}

void Tachometer::int_hall_U() {
    this->logger->debug("Pulse U");
    this->tacho_hsu.startTime = millis();

    int HSU_Val = digitalRead(IN_HS_U);
    int HSW_Val = digitalRead(IN_HS_W);
    this->tacho_direct = HSU_Val == HSW_Val ? Clock : CounterClock;

    this->tacho_hsu.pulseTime = this->tacho_hsu.startTime - this->tacho_hsu.prevTime;
    this->tacho_hsu.prevTime = this->tacho_hsu.startTime;

    //this->rpmU = self.__get_rpm(self.pulseTimeU)
    this->tacho_hsu.pulseCount = this->tacho_hsu.pulseCount + (1 * this->tacho_direct);
}

uint32_t tacho_get_rpm(uint32_t pulseTime) {
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
  this->logger->debug("Init Sensor...");

  sensor->pulseTime = 0;
  sensor->pulseCount = 0;

  sensor->startTime = 0;
  sensor->prevTime = 0;

  sensor->rpm = 0;
}

void Tachometer::hall_debug(tacho_hallSensor_t sensor) {
  this->logger->debug("{ time=%d, count=%d, rpm=%d }",
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
