/*
  mode.h - OpenWinch Project

  Copyright (C) 2020  Mickael Gaillard
*/

#ifndef MODE_HPP_
#define MODE_HPP_

class ModeType {
 public:
  enum ValueModeType {
    OneWay = 1,
    TwoWay = 2,
    Infinity = 3
  };

  ModeType() = default;
  explicit constexpr ModeType(ValueModeType aValue) : value(aValue) { }

  constexpr bool operator==(ModeType a) const { return value == a.value; }
  constexpr bool operator!=(ModeType a) const { return value != a.value; }

  ModeType list();

 private:
  ValueModeType value;

};

class ModeEngine {
 public:
  ModeEngine();
  void applyThrottleValue();
  float getDistance();
  uint32_t getSpeedCurrent();
  void runControlLoop();


 protected:
  void board = nullptr;
  void winch = nullptr;
  uint32_t speed_current = 0;

  void extraMode();
  bool isBeginSecurity();

 private:
  uint32_t security_begin = 20;
  uint32_t speed_ratio = 1;
  uint32_t velocity_start = 1;
  uint32_t velocity_stop = 3;

  void initialize();
  void starting();
  void stopping();
  void fault();
  
};

class OneWayMode: public ModeEngine {

};

class TwoWayMode: public ModeEngine {
 private:
  int security_end = 20;
  int standby_duration = 5;
  int current_duration = 0;

 protected:
  bool isEndSecurity();
  void extraMode();
};

class InfinityMode: public ModeEngine {

};

class ModeFactory {
 public:
  static ModeEngine modeFactory();
  static ModeType getMode();
}

#endif  // MODE_HPP_
