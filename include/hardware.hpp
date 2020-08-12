/*
  hardware.h - OpenWinch Project

  Copyright (C) 2020  Mickael Gaillard
*/

#ifndef HARDWARE_HPP_
#define HARDWARE_HPP_

#include <cstdint>

class Winch;
class Logger;

class SpeedMode {
 public:
  enum ValueSpeedMode : uint8_t {
    LOW = 0,
    MEDIUM = 1,
    HIGH = 2
  };

  explicit operator bool() = delete;        // Prevent usage: if(value)
  constexpr SpeedMode(const ValueSpeedMode& v) : value{v} {} //not explicit here.
  constexpr operator ValueSpeedMode() const { return value; }
  constexpr SpeedMode& operator=(ValueSpeedMode v) { value = v; return *this;}
  constexpr bool operator==(const ValueSpeedMode v) const { return value == v; }
  constexpr bool operator!=(const ValueSpeedMode v) const { return value != v; }

 private:
  ValueSpeedMode value;
  SpeedMode() = default;

};

class Board {
 public:
  Board(Winch*);
  virtual void initialize() = 0;
  virtual void emergency() = 0;
  virtual void setThrottleValue(int) = 0;
  virtual int getThrottleValue() = 0;
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
  SpeedMode speed_mode = SpeedMode::LOW;
  bool reverse = false;
  int rotation_from_init = 0;

};

class Emulator: virtual public Board {
 public:
  Emulator(Winch*);
  void initialize() override;
  void emergency() override;
  void setThrottleValue(int) override;
  int getThrottleValue() override;
  void setReverse(bool) override;
  void setSpeedMode(SpeedMode) override;
  int getBattery() override;

 private:
  int value = 0;
  bool init = false;

};


#endif  // HARDWARE_HPP_