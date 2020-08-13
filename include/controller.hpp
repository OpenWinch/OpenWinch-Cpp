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

#include <thread>

class Logger;
class Board;
class Gui;
class InputType;
class ModeEngine;
class ModeType;


class Winch {
 public:
  Winch();
  void initialize();
  void initialized();
  void start();
  void started();
  void stop();
  void stopped();
  void emergency();
  void display();
  ModeType getMode();
  uint8_t getSpeedTarget();
  State getState();
  uint8_t getBattery();
  uint8_t getRemote();
  float getDistance();
  void speedUp(uint8_t);
  void speedDown(uint8_t);
  void speedValue(uint8_t);
  void enterGui(InputType);

 private:
  Logger *logger = nullptr;
  Board *board = nullptr;
  Gui *gui = nullptr;
  InputType *input = nullptr;
  ModeEngine *mode = nullptr;
  std::thread *controlLoop;

  State state = State::UNKNOWN;
  uint8_t speed_target = SPEED_INIT;

  void banner();
  void loadConfig();
  void initControlLoop();
  void changeState(State);

};

#endif  // CONTROLLER_HPP_
