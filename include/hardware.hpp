/**
 * @file hardware.hpp
 * @author Mickael GAILLARD (mick.gaillard@gmail.com)
 * @brief OpenWinch Project
 * 
 * @copyright Copyright © 2020
 */

#ifndef HARDWARE_HPP_
#define HARDWARE_HPP_

#include <cstdint>
#include <string>

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

  operator std::string() const {
    switch (value) {
      case LOW:     return "LOW";
      case MEDIUM:  return "MEDIUM";
      case HIGH:    return "HIGH";
    }
  }

 private:
  ValueSpeedMode value;
  SpeedMode() = default;
};

class Board {
 public:
  explicit Board(Winch*);
  virtual void initialize() = 0;
  virtual void emergency() = 0;
  virtual void setThrottleValue(uint8_t) = 0;
  virtual uint8_t getThrottleValue() = 0;
  virtual void setReverse(bool) = 0;
  bool isReverse();
  virtual void setSpeedMode(SpeedMode) = 0;
  SpeedMode getSpeedMode();
  virtual uint8_t getBattery() = 0;
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
  explicit Emulator(Winch*);
  void initialize() override;
  void emergency() override;
  void setThrottleValue(uint8_t) override;
  uint8_t getThrottleValue() override;
  void setReverse(bool) override;
  void setSpeedMode(SpeedMode) override;
  uint8_t getBattery() override;

 private:
  uint8_t value = 0;
  bool init = false;

};


#endif  // HARDWARE_HPP_
