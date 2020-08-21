/**
 * @file state.hpp
 * @author Mickael GAILLARD (mick.gaillard@gmail.com)
 * @brief OpenWinch Project
 * 
 * @copyright Copyright © 2020
 */

#ifndef STATE_HPP_
#define STATE_HPP_

#ifdef __cplusplus

#include <cstdint>
#include <string>

class State {
 public:
  enum ValueState : int16_t {
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

  bool isRun();
  bool isStop();
  bool isFault();
  bool isInit();
  bool isBoot();

//   operator ValueState() const { return value; }  // Allow switch and comparisons.
//                                             // note: Putting constexpr here causes
//                                             // clang to stop warning on incomplete
//                                             // case handling.

  explicit operator bool() = delete;        // Prevent usage: if(value)
  constexpr State(const ValueState& v) : value{v} {} //not explicit here.
  constexpr operator ValueState() const { return value; }
  constexpr State& operator=(ValueState v) { value = v; return *this;}
  constexpr bool operator==(const ValueState v) const { return value == v; }
  constexpr bool operator!=(const ValueState v) const { return value != v; }

  operator std::string() const {
    switch (value) {
      case UNKNOWN: return "UNKNOWN";
      case BOOTED:  return "BOOTED";
      case ERROR:   return "ERROR";
      case INIT:    return "INIT";
      case IDLE:    return "IDLE";
      case START:   return "START";
      case RUNNING: return "RUNNING";
      case STOP:    return "STOP";
    };
  };

 private:
  ValueState value;
  State() = default;

};
#endif

#endif  // STATE_HPP_
