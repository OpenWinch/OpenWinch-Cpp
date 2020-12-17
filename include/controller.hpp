/**
 * @file controller.hpp
 * @author Mickael GAILLARD (mick.gaillard@gmail.com)
 * @brief OpenWinch Project
 * 
 * @copyright Copyright © 2020
 */

#ifndef CONTROLLER_HPP_
#define CONTROLLER_HPP_

#include "constantes.hpp"
#include "state.hpp"

class Logger;
class Board;
class Gui;
class InputType;
class Keyboard;
class ModeEngine;
class ModeType;


class Winch {
 public:
  // Winch() {};
  virtual ~Winch() {};

  virtual void boot() = 0;
  virtual void initialize() = 0;
  virtual void initialized() = 0;
  virtual void start() = 0;
  virtual void started() = 0;
  virtual void stop() = 0;
  virtual void stopped() = 0;
  virtual void emergency() = 0;
  virtual void display() = 0;
  virtual void exit() = 0;
  virtual ModeType getMode() = 0;
  virtual uint8_t getSpeedTarget() = 0;
  virtual State getState() = 0;
  virtual uint8_t getBattery() = 0;
  virtual uint8_t getRemote() = 0;
  virtual float getDistance() = 0;
  virtual void speedUp(uint8_t value = 1) = 0;
  virtual void speedDown(uint8_t value = 1) = 0;
  virtual void speedValue(uint8_t) = 0;
  virtual void enterGui(InputType) = 0;
  virtual void manualForward() = 0;
  virtual void manualReverse() = 0;
};

class WinchControl : public Winch {
 public:
  static Winch& get() {
    static WinchControl instance;
    return instance;
  }

  WinchControl();
  ~WinchControl() = default;

  void boot();
  void initialize();
  void initialized();
  void start();
  void started();
  void stop();
  void stopped();
  void emergency();
  void display();
  void exit();
  ModeType getMode();
  uint8_t getSpeedTarget();
  State getState();
  uint8_t getBattery();
  uint8_t getRemote();
  float getDistance();
  void speedUp(uint8_t value = 1);
  void speedDown(uint8_t value = 1);
  void speedValue(uint8_t);
  void enterGui(InputType);
  void manualForward();
  void manualReverse();

 protected:
  Logger* logger = nullptr;
  Board* board = nullptr;
  Gui* gui = nullptr;
  Keyboard* input = nullptr;
  ModeEngine* mode = nullptr;

  State state = State::UNKNOWN;
  uint8_t speed_target = SPEED_INIT;

  void banner();
  void loadConfig();
  void initControlLoop();
  void changeState(State);
};

#endif  // CONTROLLER_HPP_
