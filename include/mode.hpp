/*
  mode.h - OpenWinch Project

  Copyright (C) 2020  Mickael Gaillard
*/

#ifndef MODE_HPP_
#define MODE_HPP_

#include <cstdint>
#include <vector>

class Board;
class Winch;
class Logger;

class ModeType {
 public:
  enum ValueModeType : uint8_t {
    OneWay = 1,
    TwoWay = 2,
    Infinity = 3
  };

  explicit operator bool() = delete;        // Prevent usage: if(value)
  constexpr ModeType(const ValueModeType& v) : value{v} {} //not explicit here.
  constexpr operator ValueModeType() const { return value; }
  constexpr ModeType& operator=(ValueModeType v) { value = v; return *this;}
  constexpr bool operator==(const ValueModeType v) const { return value == v; }
  constexpr bool operator!=(const ValueModeType v) const { return value != v; }

  std::vector<ModeType> list();

 private:
  ValueModeType value;
  ModeType() = default;
  
};

class ModeEngine {
 public:
  explicit ModeEngine(Board*, Winch*);
  void applyThrottleValue();
  float getDistance();
  unsigned int getSpeedCurrent();
  void runControlLoop();

 protected:
  Logger *logger = nullptr;
  Board *board = nullptr;
  Winch *winch = nullptr;
  unsigned int speed_current = 0;

  virtual void extraMode() = 0;
  bool isBeginSecurity();

 private:
  unsigned int security_begin = 20;
  unsigned int speed_ratio = 1;
  unsigned int velocity_start = 1;
  unsigned int velocity_stop = 3;

  void initialize();
  void starting();
  void stopping();
  void fault();
};

class OneWayMode: public ModeEngine {
 public:
  explicit OneWayMode(Board*, Winch*);
  
 protected:
  void extraMode() override;
};

class TwoWayMode: public ModeEngine {
 public:
  explicit TwoWayMode(Board*, Winch*);

 private:
  int security_end = 20;
  int standby_duration = 5;
  int current_duration = 0;

 protected:
  bool isEndSecurity();
  void extraMode() override;
};

class InfinityMode: public ModeEngine {
 public:
  explicit InfinityMode(Board*, Winch*);

 protected:
  void extraMode() override;
};

// class ModeFactory {
//  public:
//   static ModeEngine modeFactory();
//   static ModeType getMode();
// };

#endif  // MODE_HPP_
