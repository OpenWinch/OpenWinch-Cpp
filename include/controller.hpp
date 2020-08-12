/*
  controller.h - OpenWinch Project

  Copyright (C) 2020  Mickael Gaillard
*/

#ifndef CONTROLLER_HPP_
#define CONTROLLER_HPP_

#include "constantes.hpp"
#include "display.hpp"
#include "input.hpp"
#include "mode.hpp"
#include "state.hpp"
#include "logger.hpp"
#include "hardware.hpp"

#include <thread>
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
  int getSpeedTarget();
  State getState();
  int getBattery();
  int getRemote();
  float getDistance();
  void speedUp(int);
  void speedDown(int);
  void speedValue(int);
  void enterGui(InputType);

 private:
  Logger *logger = nullptr;
  Board *board = nullptr;
  Gui *gui = nullptr;
  InputType *input = nullptr;
  ModeEngine *mode = nullptr;
  std::thread *controlLoop;

  State state = State::UNKNOWN;
  int speed_target = SPEED_INIT;

  void banner();
  void loadConfig();
  void initControlLoop();
  void changeState(State);

};

#endif  // CONTROLLER_HPP_
