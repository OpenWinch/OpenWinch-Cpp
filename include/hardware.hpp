/*
  hardware.h - OpenWinch Project

  Copyright (C) 2020  Mickael Gaillard
*/

#ifndef HARDWARE_HPP_
#define HARDWARE_HPP_

#include "controller.hpp"
#include "logger.hpp"

class SpeedMode {
 public:
  enum ValueSpeedMode {
    LOW = 0,
    MEDIUM = 1,
    HIGH = 2
  };

  SpeedMode() = default;
  explicit constexpr SpeedMode(ValueSpeedMode aValue) : value(aValue) { }

  constexpr bool operator==(SpeedMode a) const { return value == a.value; }
  constexpr bool operator!=(SpeedMode a) const { return value != a.value; }

 private:
  ValueSpeedMode value;

};

class Board {
 public:
  Board(Winch*);
  virtual void initialize() = 0;
  virtual void emergency() = 0;
  virtual void setThrottleValue(int) = 0;
  virtual float getThrottleValue() = 0;
  virtual void setReverse(bool) = 0;
  bool isReverse();
  virtual void setSpeedMode(SpeedMode) = 0;
  SpeedMode getSpeedMode();
  virtual int getBattery() = 0;
  int getRotationFromBegin();
  int getRotationFromEnd();

 protected:
  Logger *logger = nullptr;
  Winch *winch = nullptr;
  SpeedMode speed_mode; //= SpeedMode::LOW
  bool reverse = false;
  int rotation_from_init = 0;

};

class Emulator: virtual public Board {
 public:
  Emulator(Winch*);
  void initialize() override;
  void emergency() override;
  void setThrottleValue(int) override;
  float getThrottleValue() override;
  void setReverse(bool) override;
  void setSpeedMode(SpeedMode) override;
  int getBattery() override;

 private:
  int value = 0;
  bool init = false;

};


#endif  // HARDWARE_HPP_