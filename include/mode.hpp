/**
 * @file mode.hpp
 * @author Mickael GAILLARD (mick.gaillard@gmail.com)
 * @brief OpenWinch Project
 * 
 * @copyright Copyright © 2020-2021
 */

#ifndef MODE_HPP_
#define MODE_HPP_

#include <cstdint>
#include <vector>
#include <string>
#include <atomic>
#include <thread>

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

  operator std::string() const {
    std::string result("No available");

    switch (value) {
      case OneWay:   result = "OneWay"; break;
      case TwoWay:   result = "TwoWay"; break;
      case Infinity: result = "Infinity"; break;
      default: result = "Unknow";
    }

    return result;
  }

  std::vector<ModeType> list();

 private:
  ValueModeType value;
  ModeType() = default;
};

class ModeEngine {
 public:
  explicit ModeEngine(Board*, Winch*);

  void run();
  void abort();
  bool isRunning() const;
  virtual ~ModeEngine() = default;

  void applyThrottleValue();
  float getDistance();
  uint8_t getSpeedCurrent();
  void runControlLoop();

 protected:
  Logger* logger = nullptr;
  Board* board = nullptr;
  Winch* winch = nullptr;
  uint8_t speed_current = 0;

  virtual void extraMode() = 0;
  bool isBeginSecurity();

 private:
  std::atomic_bool abortRequested = ATOMIC_VAR_INIT(false);
  std::atomic_bool running = ATOMIC_VAR_INIT(false);
  std::thread controlLoop;

  uint8_t security_begin = 20;
  uint8_t speed_ratio = 1;
  uint8_t velocity_start = 1;
  uint8_t velocity_stop = 3;

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
  uint8_t security_end = 20;
  uint8_t standby_duration = 5;
  uint8_t current_duration = 0;

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
