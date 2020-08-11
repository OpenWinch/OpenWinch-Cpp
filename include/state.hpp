/*
  state.h - OpenWinch Project

  Copyright (C) 2020  Mickael Gaillard
*/

#ifndef STATE_HPP_
#define STATE_HPP_

#ifdef __cplusplus

class State {
 public:
  enum ValueState {
    UNKNOWN = -999,
    BOOTED = -2,
    ERROR = -1,
    INIT = 0,
    IDLE = 1,
    START = 2,
    RUNNING = 3,
    STOP = 4
  };

  static bool checkRun(State current);
  static bool checkStop(State current);
  static bool checkFault(State current);
  static bool checkInit(State current);
  static bool checkBoot(State current);

  State() = default;
  explicit constexpr State(ValueState aValue) : value(aValue) { }

  bool isRun();
  bool isStop();
  bool isFault();
  bool isInit();
  bool isBoot();

// #if Enable switch (State) use case:
//   operator Value() const { return value; }  // Allow switch and comparisons.
//                                             // note: Putting constexpr here causes
//                                             // clang to stop warning on incomplete
//                                             // case handling.
//   explicit operator bool() = delete;        // Prevent usage: if(fruit)
// #else
  constexpr bool operator==(State a) const { return value == a.value; }
  constexpr bool operator!=(State a) const { return value != a.value; }
// #endif

 private:
  ValueState value;

};
#endif

#endif  // STATE_HPP_
